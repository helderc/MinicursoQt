#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_actionAbrir_Imagem_triggered();

    void on_actionSair_triggered();

    void on_actionSalvar_como_triggered();

private:
    Ui::MainWindow *ui;
    QImage *img;
    cv::Mat matimg;

    void salvarComo();
};

#endif // MAINWINDOW_H
