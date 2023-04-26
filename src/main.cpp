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






int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
