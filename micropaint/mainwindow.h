#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QStyleFactory>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QColorDialog>

#include "graphicsscene.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void starting();

    enum TipoItem { Lapis = 2, Quadrado = 3, Circulo = 4, Line = 6, Texto = 8};

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    GraphicsScene *graphicsScene;


private slots:
    void on_actionAbout_Qt_triggered();
    void on_actionExit_triggered();
    void on_actionCirc_triggered();
    void on_actionSelect_triggered();
    void on_actionLine_triggered();
    void on_actionSquare_triggered();
    void on_actionText_triggered();
    void on_actionDraw_triggered();
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionTrazer_para_frente_triggered();

    void contextMenu(const QPoint& p);
    void on_actionEnviar_para_tr_s_triggered();
};


#endif // MAINWINDOW_H
