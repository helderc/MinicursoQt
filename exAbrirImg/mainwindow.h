#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void tonsCinza(QImage *img);
    void imgUpdate();
    
private slots:
    void on_actionAbrir_Imagem_triggered();

    void on_actionSair_triggered();

    void on_actionTonsCinza_triggered();

private:
    Ui::MainWindow *ui;
    QImage *img;
};

#endif // MAINWINDOW_H
