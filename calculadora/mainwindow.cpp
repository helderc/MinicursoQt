#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ActionGroup: Faz com que as entradas sejam auto-exclusivas
    QActionGroup *actionGroup = new QActionGroup(this);
    actionGroup->addAction(ui->actionCient_fica);
    actionGroup->addAction(ui->actionPadr_o);

    connect(ui->actionCient_fica, SIGNAL(toggled(bool)),
            ui->widgetConv, SLOT(setVisible(bool)));

    inicializa();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::inicializa()
{
    esperandoNum = true;

    valorEsquerda = 0;
    valorDireita = 0;
    valorResultado = 0;

    opSelecionada = "";

    ui->visor->setText("0");

    ui->rdbDec->setChecked(true);
}

void MainWindow::digitoClick()
{
    QPushButton * btn = qobject_cast<QPushButton *>(sender());

    if (esperandoNum) {
        ui->visor->clear();
        esperandoNum = false;
    }

    ui->visor->setText(ui->visor->text() + btn->text());
}

void MainWindow::opClick()
{
    QPushButton * btn = qobject_cast<QPushButton *>(sender());

    // Operacao Selecionada
    opSelecionada = btn->text();

    // Numero a Esquerda
    valorEsquerda = ui->visor->text().toDouble();

    esperandoNum = true;
}

void MainWindow::igualClick()
{
    // Numero da Direita
    valorDireita = ui->visor->text().toDouble();

    // Executa de acordo com a Op Selecionada
    if (opSelecionada == "+")
        valorResultado = valorEsquerda + valorDireita;
    else if (opSelecionada == "-")
        valorResultado = valorEsquerda - valorDireita;
    else if (opSelecionada == "*")
        valorResultado = valorEsquerda * valorDireita;
    else if (opSelecionada == "/")
        valorResultado = valorEsquerda / valorDireita;
    else
        qDebug() << "Op Invalida!";

    ui->visor->setText(QString::number(valorResultado));
}



void MainWindow::on_btnLimpar_clicked()
{
    inicializa();
}

void MainWindow::on_btnVirg_clicked()
{
    if (esperandoNum)
        ui->visor->setText("0");

    if (!ui->visor->text().contains(","))
        ui->visor->setText(ui->visor->text() + ",");

    esperandoNum = false;
}

void MainWindow::on_btnBackSpace_clicked()
{
    if (esperandoNum)
        return;

    QString text = ui->visor->text();

    // Remove 1 caractere do final da String
    text.chop(1);

    if (text.isEmpty()) {
        text = "0";
        esperandoNum = true;
    }

    ui->visor->setText(text);
}

void MainWindow::convSistemas()
{
    long valorInicial = ui->visor->text().toLong();
    QString valorConvertido;
    int sistema;

    // Decimal
    if (ui->rdbDec->isChecked())
        sistema = 10;
    // Binario
    else if (ui->rdbBin->isChecked())
        sistema = 2;
    // Hexadecimal
    else if (ui->rdbHex->isChecked())
        sistema = 16;
    // Octal
    else if (ui->rdbOct->isChecked())
        sistema = 8;
    else
        return;

    valorConvertido = QString::number(valorInicial, sistema).toUpper();

    ui->visor->setText(valorConvertido);
}
