#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Eigen>

#include "../include/global.hpp"
#include "../include/rasterizer.hpp"
#include "../include/Triangle.hpp"
#include "../include/Shader.hpp"
#include "../include/Texture.hpp"
#include "../include/OBJ_Loader.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString modelPath;
    QString texturePath;
    QString normalPath;
    int clicked=0;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos);
    Eigen::Matrix4f get_model_matrix(float angle);
    Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar);

    
    void drawPipeline();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_texture_clicked();

    void on_normal_clicked();

    void on_blinnPhong_clicked();

    void on_noBlinnPhong_clicked();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H

static Eigen::Vector3f reflect(const Eigen::Vector3f& vec, const Eigen::Vector3f& axis);
Eigen::Vector3f texture_fragment_shader(const fragment_shader_payload& payload);
Eigen::Vector3f phong_fragment_shader(const fragment_shader_payload& payload);
Eigen::Vector3f displacement_fragment_shader(const fragment_shader_payload& payload);
Eigen::Vector3f bump_fragment_shader(const fragment_shader_payload& payload);
Eigen::Vector3f normal_fragment_shader(const fragment_shader_payload& payload);
Eigen::Vector3f vertex_shader(const vertex_shader_payload& payload);