#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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
