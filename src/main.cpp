#include "../include/mainwindow.h"

#include <QApplication>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "../include/global.hpp"
#include "../include/rasterizer.hpp"
#include "../include/Triangle.hpp"
#include "../include/Shader.hpp"
#include "../include/Texture.hpp"
#include "../include/OBJ_Loader.h"

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1,0,0,-eye_pos[0],
                 0,1,0,-eye_pos[1],
                 0,0,1,-eye_pos[2],
                 0,0,0,1;

    view = translate*view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float angle)
{
    Eigen::Matrix4f rotation;
    angle = angle * MY_PI / 180.f;
    rotation << cos(angle), 0, sin(angle), 0,
                0, 1, 0, 0,
                -sin(angle), 0, cos(angle), 0,
                0, 0, 0, 1;

    Eigen::Matrix4f scale;
    scale << 2.5, 0, 0, 0,
              0, 2.5, 0, 0,
              0, 0, 2.5, 0,
              0, 0, 0, 1;

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;

    return translate * rotation * scale;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar)
{
    // TODO: Use the same projection matrix from the previous assignments
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
    Matrix4f pro(4, 4);
    //透视变换
    float n = zNear;
    float f = zFar;
    pro << n, 0, 0, 0,
        0, n, 0, 0,
        0, 0, n + f, -n * f,
        0, 0, 1, 0;
    Matrix4f orth(4, 4);
 
    float halfAngle = eye_fov * MY_PI / 180.0f;

    auto top = -zNear * tan(halfAngle / 2);
    auto right = top * aspect_ratio;
    auto left = -right;
    auto bottom = -top;
 
    //平移矩阵
    Matrix4f move(4, 4);
    move << 1,0,0,-(right+left)/2,
            0,1,0,-(top + bottom)/2,
            0,0,1,-(zFar+zNear)/2,
            0,0,0,1;
    //缩放矩阵
    orth <<2/(right-left),0,0,0,
            0,2/(top-bottom),0,0,
            0,0,2/(zNear-zFar),0,
            0,0,0,1;
 
    //注意乘积的顺序
    projection = orth * move * pro;

    return projection;

}

Eigen::Vector3f vertex_shader(const vertex_shader_payload& payload)
{
    return payload.position;
}

//color the object using normal
Eigen::Vector3f normal_fragment_shader(const fragment_shader_payload& payload)
{
    Eigen::Vector3f return_color = (payload.normal.head<3>().normalized() + Eigen::Vector3f(1.0f, 1.0f, 1.0f)) / 2.f;
    Eigen::Vector3f result;
    result << return_color.x() * 255, return_color.y() * 255, return_color.z() * 255;
    return result;
}

static Eigen::Vector3f reflect(const Eigen::Vector3f& vec, const Eigen::Vector3f& axis)
{
    auto costheta = vec.dot(axis);
    return (2 * costheta * axis - vec).normalized();
}

struct light
{
    Eigen::Vector3f position;
    Eigen::Vector3f intensity;
};

//color the object using texture
Eigen::Vector3f texture_fragment_shader(const fragment_shader_payload& payload)
{
    Eigen::Vector3f return_color = {0, 0, 0};
    if (payload.texture)
    {
        // TODO: Get the texture value at the texture coordinates of the current fragment
        return_color=payload.texture->getColor(payload.tex_coords.x(),payload.tex_coords.y());
    }
    Eigen::Vector3f texture_color;
    texture_color << return_color.x(), return_color.y(), return_color.z();

    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd = texture_color / 255.f;
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Eigen::Vector3f amb_light_intensity{10, 10, 10};
    Eigen::Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Eigen::Vector3f color = texture_color;
    Eigen::Vector3f point = payload.view_pos;
    Eigen::Vector3f normal = payload.normal;

    Eigen::Vector3f result_color = {0, 0, 0};

    for (auto& light : lights)
    {
        // TODO: For each light source in the code, calculate what the *ambient*, *diffuse*, and *specular* 
        // components are. Then, accumulate that result on the *result_color* object.
        Eigen::Vector3f light_dir=light.position-point;
        Eigen::Vector3f view_dir=eye_pos-point;

        //ambient
        Eigen::Vector3f la=ka.cwiseProduct(amb_light_intensity);

        //diffuse   cosine is two normalize vector do dot production
        Eigen::Vector3f ld=kd.cwiseProduct(light.intensity/light_dir.dot(light_dir));
        ld*=std::max(0.0f,normal.normalized().dot(light_dir.normalized()));

        //specular
        auto helf_vector=(view_dir+light_dir).normalized();
        Eigen::Vector3f ls=ks.cwiseProduct(light.intensity/light_dir.dot(light_dir));
        ls*=std::pow(std::max(0.0f,normal.normalized().dot(helf_vector)),p);

        result_color += (la + ld + ls);        

    }

    return result_color * 255.f;
}

Eigen::Vector3f phong_fragment_shader(const fragment_shader_payload& payload)
{
    //ka: ambient coefficient 
    //kb: diffuse coefficient
    //ks: specular coefficient
    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd = payload.color;
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

    //define lighting
    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Eigen::Vector3f amb_light_intensity{10, 10, 10};
    Eigen::Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Eigen::Vector3f color = payload.color;
    Eigen::Vector3f point = payload.view_pos;
    Eigen::Vector3f normal = payload.normal;

    Eigen::Vector3f result_color = {0, 0, 0};
    for (auto& light : lights)
    {
        // TODO: For each light source in the code, calculate what the *ambient*, *diffuse*, and *specular* 
        // components are. Then, accumulate that result on the *result_color* object.

        Eigen::Vector3f light_dir=light.position-point;
        Eigen::Vector3f view_dir=eye_pos-point;

        //ambient
        Eigen::Vector3f la=ka.cwiseProduct(amb_light_intensity);

        //diffuse   cosine is two normalize vector do dot production
        Eigen::Vector3f ld=kd.cwiseProduct(light.intensity/light_dir.dot(light_dir));
        ld*=std::max(0.0f,normal.normalized().dot(light_dir.normalized()));

        //specular
        auto helf_vector=(view_dir+light_dir).normalized();
        Eigen::Vector3f ls=ks.cwiseProduct(light.intensity/light_dir.dot(light_dir));
        ls*=std::pow(std::max(0.0f,normal.normalized().dot(helf_vector)),p);

        result_color += (la + ld + ls);        
    }

    return result_color * 255.f;
}



Eigen::Vector3f displacement_fragment_shader(const fragment_shader_payload& payload)
{
    
    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd = payload.color;
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Eigen::Vector3f amb_light_intensity{10, 10, 10};
    Eigen::Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Eigen::Vector3f color = payload.color; 
    Eigen::Vector3f point = payload.view_pos;
    Eigen::Vector3f normal = payload.normal;

    float kh = 0.2, kn = 0.1;
    
    // TODO: Implement displacement mapping here
    // Let n = normal = (x, y, z)
    // Vector t = (x*y/sqrt(x*x+z*z),sqrt(x*x+z*z),z*y/sqrt(x*x+z*z))
    // Vector b = n cross product t
    // Matrix TBN = [t b n]
    // dU = kh * kn * (h(u+1/w,v)-h(u,v))
    // dV = kh * kn * (h(u,v+1/h)-h(u,v))
    // Vector ln = (-dU, -dV, 1)
    // Position p = p + kn * n * h(u,v)
    // Normal n = normalize(TBN * ln)
    float x,y,z;
    x = normal.x();
    y = normal.y();
    z = normal.z();
    normal=Eigen::Vector3f(x,y,z);
    Eigen::Vector3f t=Eigen::Vector3f(x * y / sqrt(x * x + z * z), sqrt(x * x + z * z), z * y / sqrt(x * x + z * z));
    Eigen::Vector3f b = normal.cross(t);
    Matrix3f TBN(3, 3);
    TBN <<  t.x(),b.x(),normal.x(),
            t.y(),b.y(),normal.y(),
            t.z(),b.z(),normal.z();

    float u, v, w, h;
    u = payload.tex_coords.x();
    v = payload.tex_coords.y();
    w = payload.texture->width;
    h = payload.texture->height;
    float dU = kh * kn * (payload.texture->getColor(u+1.0/w,v).norm() - payload.texture->getColor(u,v).norm());
    float dV = kh * kn * (payload.texture->getColor(u,v+1.0/h).norm() - payload.texture->getColor(u,v).norm());
    Eigen::Vector3f ln=Eigen::Vector3f(-dU,-dV,1);

    point += kn * normal * payload.texture->getColor(u,v).norm();//这一步就是区分displacement与bump的关键，位移
    normal = (TBN * ln).normalized();

    Eigen::Vector3f result_color = {0, 0, 0};

    for (auto& light : lights)
    {
        // TODO: For each light source in the code, calculate what the *ambient*, *diffuse*, and *specular* 
        // components are. Then, accumulate that result on the *result_color* object.
        Eigen::Vector3f light_dir=light.position-point;
        Eigen::Vector3f view_dir=eye_pos-point;

        //ambient
        Eigen::Vector3f la=ka.cwiseProduct(amb_light_intensity);

        //diffuse   cosine is two normalize vector do dot production
        Eigen::Vector3f ld=kd.cwiseProduct(light.intensity/light_dir.dot(light_dir));
        ld*=std::max(0.0f,normal.normalized().dot(light_dir.normalized()));

        //specular
        auto helf_vector=(view_dir+light_dir).normalized();
        Eigen::Vector3f ls=ks.cwiseProduct(light.intensity/light_dir.dot(light_dir));
        ls*=std::pow(std::max(0.0f,normal.normalized().dot(helf_vector)),p);

        result_color += (la + ld + ls);   

    }

    return result_color * 255.f;
}


Eigen::Vector3f bump_fragment_shader(const fragment_shader_payload& payload)
{
    
    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd = payload.color;
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Eigen::Vector3f amb_light_intensity{10, 10, 10};
    Eigen::Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Eigen::Vector3f color = payload.color; 
    Eigen::Vector3f point = payload.view_pos;
    Eigen::Vector3f normal = payload.normal;


    float kh = 0.2, kn = 0.1;

    // TODO: Implement bump mapping here
    // Let n = normal = (x, y, z)
    // Vector t = (x*y/sqrt(x*x+z*z),sqrt(x*x+z*z),z*y/sqrt(x*x+z*z))
    // Vector b = n cross product t
    // Matrix TBN = [t b n]
    // dU = kh * kn * (h(u+1/w,v)-h(u,v))
    // dV = kh * kn * (h(u,v+1/h)-h(u,v))
    // Vector ln = (-dU, -dV, 1)
    // Normal n = normalize(TBN * ln)

    float x,y,z;
    x = normal.x();
    y = normal.y();
    z = normal.z();
    normal=Eigen::Vector3f(x,y,z);
    Eigen::Vector3f t=Eigen::Vector3f(x * y / sqrt(x * x + z * z), sqrt(x * x + z * z), z * y / sqrt(x * x + z * z));
    Eigen::Vector3f b = normal.cross(t);
    Matrix3f TBN(3, 3);
    TBN <<  t.x(),b.x(),normal.x(),
            t.y(),b.y(),normal.y(),
            t.z(),b.z(),normal.z();

    float u, v, w, h;
    u = payload.tex_coords.x();
    v = payload.tex_coords.y();
    w = payload.texture->width;
    h = payload.texture->height;
    float dU = kh * kn * (payload.texture->getColor(u+1.0/w,v).norm() - payload.texture->getColor(u,v).norm());
    float dV = kh * kn * (payload.texture->getColor(u,v+1.0/h).norm() - payload.texture->getColor(u,v).norm());
    Eigen::Vector3f ln=Eigen::Vector3f(-dU,-dV,1);

    normal = (TBN * ln).normalized();

    Eigen::Vector3f result_color = {0, 0, 0};
    result_color = normal;

    return result_color * 255.f;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
