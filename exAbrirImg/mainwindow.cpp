#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::tonsCinza(QImage *img)
{
    int cinza, vermelho, verde, azul;
    QRgb cor;

    for (int i = 0; i < img->width(); i++)
        for (int j = 0; j < img->height(); j++) {

            cor = img->pixel(QPoint(i, j));

            vermelho = qRed(cor);
            verde = qGreen(cor);
            azul = qBlue(cor);

            cinza = (vermelho + verde + azul) / 3;

            img->setPixel(QPoint(i, j), qRgb(cinza, cinza, cinza));
        }
}

void MainWindow::imgUpdate()
{
    // coloca a imagem no Label para visualizar
    ui->lblImg->setPixmap(QPixmap::fromImage(*img));
}

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
        // cria um QImage do arquivo especificado
        img = new QImage(fileName);

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

void MainWindow::on_actionTonsCinza_triggered()
{
    tonsCinza(img);
    imgUpdate();
}
