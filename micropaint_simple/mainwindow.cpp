#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // criacao da cena com 5000x5000, cor de fundo RGB(242,251,235)
    graphicsScene = new GraphicsScene(this);
    graphicsScene->setSceneRect(0, 0, 5000, 5000);
    graphicsScene->setBackgroundBrush(QBrush(qRgb(242,251,235)));

    // configuracoes do view
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    ui->graphicsView->setScene(graphicsScene);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_actionSelect_triggered()
{
    // necessario definir novamente o RubberBandDrag para poder selecionar varios itens
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    graphicsScene->setMode(GraphicsScene::MoveItem);
}

void MainWindow::on_actionCirc_triggered()
{
    ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
    graphicsScene->setMode(GraphicsScene::InsCirc);
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionLine_triggered()
{
    ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
    graphicsScene->setMode(GraphicsScene::InsLine);
}


void MainWindow::on_actionNew_triggered()
{
    graphicsScene->novoDesenho();
}

void MainWindow::on_actionSave_triggered()
{
    QImage newImage(graphicsScene->itemsBoundingRect().size().toSize(), QImage::Format_RGB32);
    QPainter painter(&newImage);

    // para uma melhor renderizacao no arquivo final
    painter.setRenderHint(QPainter::Antialiasing);

    // renderiza a cena para que possa ser salva como imagem
    graphicsScene->render(&painter, QRectF(), graphicsScene->itemsBoundingRect());

    // mostra a janela de salvar arquivo
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Salvar Imagem"),
                                                    NULL,
                                                    "Imagem PNG (*.png)");

    if (!fileName.isNull()) {
        newImage.save(fileName, "PNG");
        QMessageBox::information(this, "Salvar Imagem", "Imagem Salva", QMessageBox::Ok);
    } else
        QMessageBox::critical(this, "Salvar Imagem", "Nao salvou!", QMessageBox::Ok);
}
