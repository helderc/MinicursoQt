#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionAbrir_Imagem_triggered()
{
    // caixa de abertura de arquivo
    QString fileName = QFileDialog::getOpenFileName(this, tr("Abrir Imagem"));

    if (!fileName.isEmpty()) {
        // abre a imagem no formato do OpenCV
        matimg = cv::imread(fileName.toStdString().data());

        // Rotaciona a Imagem
        cv::flip(matimg, matimg, 0);

        // Muda a ordem do canal de cor BGR -> RGB
        cv::cvtColor(matimg, matimg, CV_BGR2RGB);

        // Convertendo para uma estrutura QImage.
        // cv::Mat -> QImage
        img = new QImage((const unsigned char*)(matimg.data),
                     matimg.cols, matimg.rows,
                     QImage::Format_RGB888);

        // coloca a imagem no Label para visualizar
        ui->lblImg->setPixmap(QPixmap::fromImage(*img));

        // mostrar msg na barra de status
        ui->statusBar->showMessage(fileName);
    }
}

void MainWindow::on_actionSair_triggered()
{
    exit(0);
}

void MainWindow::salvarComo()
{
    QString outputFileName = QFileDialog::getSaveFileName(this,
                                                          tr("Save As"),
                                                          "",
                                                          tr("Image Files (*.png *.jpg *.jpeg *.bmp)\n *.png\n *.jpg\n *.jpeg\n *.bmp"));

    // QImage -> Mat
    cv::Mat mat = cv::Mat(img->height(), img->width(),
                          CV_8UC3, (uchar*)img->bits(), img->bytesPerLine());

    cv::cvtColor(mat, mat, CV_BGR2RGB);
    cv::imwrite(outputFileName.toStdString().data(), mat);
}



void MainWindow::on_actionSalvar_como_triggered()
{
    salvarComo();
}
