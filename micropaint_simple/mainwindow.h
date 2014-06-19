#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
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

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    GraphicsScene *graphicsScene;


private slots:
    void on_actionExit_triggered();
    void on_actionCirc_triggered();
    void on_actionSelect_triggered();
    void on_actionLine_triggered();
    void on_actionNew_triggered();
    void on_actionSave_triggered();
};


#endif // MAINWINDOW_H
