//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H
#include "global.hpp"
#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>
class Texture{
private:
    cv::Mat image_data;

public:
    Texture(const std::string& name)
    {
        image_data = cv::imread(name);
        cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    int width, height;

    Eigen::Vector3f getColor(float u, float v)
    {
        u = std::fmin(1, std::fmax(u, 0));
        v = std::fmin(1, std::fmax(v, 0));

        auto u_img = u * width;
        auto v_img = (1 - v) * height;
        auto color = image_data.at<cv::Vec3b>(v_img, u_img);
        return Eigen::Vector3f(color[0], color[1], color[2]);
    }

    Eigen::Vector3f getColorBilinear(float u, float v)
    {
        u = std::fmin(1, std::fmax(u, 0));
        v = std::fmin(1, std::fmax(v, 0));

        //identify four pixels
        auto u1_img = int(u * width);
        auto v1_img = int(v * height);
        auto u2_img = u1_img+1;
        auto v2_img = v1_img;
        auto u3_img = u1_img;
        auto v3_img = v1_img+1;
        auto u4_img = u1_img+1;
        auto v4_img = v1_img+1;

        //four pixel color
        auto color1=getColor(u1_img/width,v1_img/height);
        auto color2=getColor(u2_img/width,v2_img/height);
        auto color3=getColor(u3_img/width,v3_img/height);
        auto color4=getColor(u4_img/width,v4_img/height);

        //Bilinear
        auto color_1=color1+(color2-color1)*(u*width-u1_img);
        auto color_2=color3+(color4-color3)*(u*width-u1_img);
        auto result=color_1+(color_2-color_1)*(v*height-v4_img);

        return result;
    }

};
#endif //RASTERIZER_TEXTURE_H
