#include "../include/mainwindow.h"
#include "./ui_mainwindow.h"
#include<QFileDialog>
#include<QMessageBox>
#include<QDesktopServices>
#include<QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,tr("Open File"),"", "Obj Files (*.obj*)");
    if (!filename.isEmpty()) {
        ui->lineEdit->setText(filename);
    }
    modelPath=filename;
}


void MainWindow::on_pushButton_2_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,tr("Open File"),"", "Jpg Files (*.jpg*)");
    if (!filename.isEmpty()) {
        ui->lineEdit_2->setText(filename);
    }
    normalPath=filename;
}


void MainWindow::on_pushButton_3_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,tr("Open File"),"", "Jpg Files (*.jpg*)");
    if (!filename.isEmpty()) {
        ui->lineEdit_3->setText(filename);
    }
    texturePath=filename;
}


void MainWindow::on_texture_clicked()
{
    QPixmap pix("/Users/naname/Documents/GAMES101/Assignment3/Pipeline-and-Shading/build/output.png");
    ui->image->setPixmap(pix);
}


void MainWindow::on_normal_clicked()
{
    QPixmap pix("/Users/naname/Documents/GAMES101/Assignment3/Pipeline-and-Shading/build/output.png");
    ui->image->setPixmap(pix);
}


void MainWindow::on_blinnPhong_clicked()
{
    QPixmap pix("/Users/naname/Documents/GAMES101/Assignment3/Pipeline-and-Shading/build/output.png");
    ui->image->setPixmap(pix);
}


void MainWindow::on_noBlinnPhong_clicked()
{
    QPixmap pix("/Users/naname/Documents/GAMES101/Assignment3/Pipeline-and-Shading/build/output.png");
    ui->image->setPixmap(pix);
}

