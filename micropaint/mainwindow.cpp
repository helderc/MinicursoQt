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
    // a operacao padrao da cena: MoverItem
    graphicsScene->setMode(GraphicsScene::MoveItem);

    // configuracoes do view
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    ui->graphicsView->setScene(graphicsScene);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    // possibilita o uso de menu de contexto no view
    ui->graphicsView->setContextMenuPolicy(Qt::CustomContextMenu);
    // conecta o sinal emitido pelo graphicsview ao metodo responsavel por criar o menu
    connect(ui->graphicsView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(contextMenu(QPoint)));
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

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, "About Qt... ");
}

void MainWindow::on_actionLine_triggered()
{
    ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
    graphicsScene->setMode(GraphicsScene::InsLine);
}

void MainWindow::on_actionSquare_triggered()
{
    ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
    graphicsScene->setMode(GraphicsScene::InsQuad);
}

void MainWindow::on_actionText_triggered()
{
    ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
    graphicsScene->setMode(GraphicsScene::InsText);
}

void MainWindow::on_actionDraw_triggered()
{
    ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
    graphicsScene->setMode(GraphicsScene::InsDraw);

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
        newImage.save(fileName + ".png", "PNG");
        QMessageBox::information(this, "Salvar Imagem", "Imagem Salva", QMessageBox::Ok);
    } else
        QMessageBox::critical(this, "Salvar Imagem", "Nao salvou!", QMessageBox::Ok);
}

void MainWindow::contextMenu(const QPoint &p)
{
    // mostra o menu de contexto para o item na posicao "p"
    QGraphicsItem *item = ui->graphicsView->itemAt(p);

    if (item) {
        // o menu deve ser mostrado na posicao onde o mouse foi clicado
        QPoint globalPos = ui->graphicsView->mapToGlobal(p);
        QMenu menu;

        QMenu *linha = new QMenu("Linha/Texto", &menu);
        QMenu *preenchimento = new QMenu("Preenchimento", &menu);
        QMenu *menuLinha = new QMenu("Estilo", linha);
        QMenu *menuPadroes = new QMenu("Padrões", preenchimento);
        QMenu *menuEspessura = new QMenu("Espessura", linha);

        QAction* estilo1 = new QAction(" - - - - - ", menuLinha);
        QAction* estilo2 = new QAction(" - . - . - ", menuLinha);
        QAction* estilo3 = new QAction(" - .. - .. - ", menuLinha);

        menuLinha->addAction(estilo1);
        menuLinha->addAction(estilo2);
        menuLinha->addAction(estilo3);

        QAction* estiloPadrao1 = new QAction("Linhas horizontais", menuPadroes);
        QAction* estiloPadrao2 = new QAction("Linhas verticais", menuPadroes);
        QAction* estiloPadrao3 = new QAction("Quadriculado", menuPadroes);

        menuPadroes->addAction(estiloPadrao1);
        menuPadroes->addAction(estiloPadrao2);
        menuPadroes->addAction(estiloPadrao3);

        QAction* espessura1 = new QAction("2", menuEspessura);
        QAction* espessura2 = new QAction("3", menuEspessura);
        QAction* espessura3 = new QAction("4", menuEspessura);

        menuEspessura->addAction(espessura1);
        menuEspessura->addAction(espessura2);
        menuEspessura->addAction(espessura3);

        QAction* corLinha = new QAction("Cor", linha);
        QAction* corPreen = new QAction("Cor", preenchimento);

        linha->addAction(corLinha);
        linha->addAction(menuLinha->menuAction());
        linha->addAction(menuEspessura->menuAction());

        preenchimento->addAction(menuPadroes->menuAction());
        preenchimento->addAction(corPreen);

        menu.addAction(linha->menuAction());
        menu.addAction(preenchimento->menuAction());

        QAction* selectedItem;
        int tipoItem = item->type();

        // seleciona o que vai ficar Habilitado/Desabilitado no menu baseado no item selecionado
        if (tipoItem == MainWindow::Line) {
            if (QGraphicsItemGroup *g = item->group()) {
                preenchimento->setEnabled(false);
                menuLinha->setEnabled(false);

                // pega a entrada do menu que foi selecionada
                selectedItem = menu.exec(globalPos);
                if (selectedItem) {

                    // Alterar Cor da Linha
                    if (selectedItem == corLinha) {
                        QColor cor = QColorDialog::getColor();
                        if (cor.isValid())
                           foreach (QGraphicsItem* i, g->childItems())
                               (static_cast<QGraphicsLineItem*>(i))->setPen(QPen(cor));

                    // Alterar Espessura da Linha: 2
                    } else if (selectedItem == espessura1)  {
                        QList<QGraphicsItem*> itens = g->childItems();
                        QPen currentPen = (static_cast<QGraphicsLineItem*>(itens[0]))->pen();
                        foreach (QGraphicsItem* i, itens)
                            (static_cast<QGraphicsLineItem*>(i))->setPen(QPen(currentPen.brush(), 2));

                    // Alterar Espessura da Linha: 3
                    } else if (selectedItem == espessura2) {
                        QList<QGraphicsItem*> itens = g->childItems();
                        QPen currentPen = (static_cast<QGraphicsLineItem*>(itens[0]))->pen();
                        foreach (QGraphicsItem* i, itens)
                            (static_cast<QGraphicsLineItem*>(i))->setPen(QPen(currentPen.brush(), 3));

                    // Alterar Espessura da Linha: 4
                    } else if (selectedItem == espessura3) {
                        QList<QGraphicsItem*> itens = g->childItems();
                        QPen currentPen = (static_cast<QGraphicsLineItem*>(itens[0]))->pen();
                        foreach (QGraphicsItem* i, itens)
                            (static_cast<QGraphicsLineItem*>(i))->setPen(QPen(currentPen.brush(), 4));
                    }

                }

           } else {
               preenchimento->setEnabled(false);
               // devo recuperar aqui dentro, pois preciso desabilitar algumas opções
               selectedItem = menu.exec(globalPos);

               if (selectedItem) {
                   if (selectedItem == estilo1) {
                       QPen currentPen = (static_cast<QGraphicsLineItem*>(item))->pen();

                       currentPen.setStyle(Qt::DashLine);
                       (static_cast<QGraphicsLineItem*>(item))->setPen(currentPen);

                   } else if (selectedItem == estilo2) {
                       QPen currentPen = (static_cast<QGraphicsLineItem*>(item))->pen();

                       currentPen.setStyle(Qt::DashDotLine);
                       (static_cast<QGraphicsLineItem*>(item))->setPen(currentPen);

                   } else if (selectedItem == estilo3) {
                       QPen currentPen = (static_cast<QGraphicsLineItem*>(item))->pen();

                       currentPen.setStyle(Qt::DashDotDotLine);
                       (static_cast<QGraphicsLineItem*>(item))->setPen(currentPen);


                   } else if (selectedItem == corLinha) {
                       QColor cor = QColorDialog::getColor();

                       if (cor.isValid()) {
                           QPen currentPen = (static_cast<QGraphicsLineItem*>(item))->pen();
                           currentPen.setColor(cor);
                           (static_cast<QGraphicsLineItem*>(item))->setPen(currentPen);
                       }

                   } else if (selectedItem == espessura1) {

                       QPen currentPen = (static_cast<QGraphicsLineItem*>(item))->pen();

                       (static_cast<QGraphicsLineItem*>(item))->setPen(QPen(currentPen.brush(), 2));

                   } else if (selectedItem == espessura2) {

                       QPen currentPen = (static_cast<QGraphicsLineItem*>(item))->pen();

                       (static_cast<QGraphicsLineItem*>(item))->setPen(QPen(currentPen.brush(), 3));

                   } else if (selectedItem == espessura3) {

                       QPen currentPen = (static_cast<QGraphicsLineItem*>(item))->pen();

                       (static_cast<QGraphicsLineItem*>(item))->setPen(QPen(currentPen.brush(), 4));

                   }
               }


           }
       }
       else {
           if (tipoItem == MainWindow::Quadrado) {

               selectedItem = menu.exec(globalPos);

               if (selectedItem) {
                   if (selectedItem == corLinha) {
                       QColor cor = QColorDialog::getColor();

                       if (cor.isValid()) {
                           QPen currentPen = (static_cast<QGraphicsRectItem*>(item))->pen();
                           currentPen.setColor(cor);
                           (static_cast<QGraphicsRectItem*>(item))->setPen(currentPen);
                       }

                   } else if (selectedItem == estilo1) {
                       QPen currentPen = (static_cast<QGraphicsRectItem*>(item))->pen();

                       currentPen.setStyle(Qt::DashLine);
                       (static_cast<QGraphicsRectItem*>(item))->setPen(currentPen);

                   } else if (selectedItem == estilo2) {
                       QPen currentPen = (static_cast<QGraphicsRectItem*>(item))->pen();

                       currentPen.setStyle(Qt::DashDotLine);
                       (static_cast<QGraphicsRectItem*>(item))->setPen(currentPen);

                   } else if (selectedItem == estilo3) {
                       QPen currentPen = (static_cast<QGraphicsRectItem*>(item))->pen();

                       currentPen.setStyle(Qt::DashDotLine);
                       (static_cast<QGraphicsRectItem*>(item))->setPen(currentPen);

                   } else if (selectedItem == espessura1) {
                       QPen currentPen = (static_cast<QGraphicsRectItem*>(item))->pen();

                       currentPen.setWidth(2);
                       (static_cast<QGraphicsRectItem*>(item))->setPen(currentPen);

                   } else if (selectedItem == espessura2) {
                       QPen currentPen = (static_cast<QGraphicsRectItem*>(item))->pen();

                       currentPen.setWidth(3);
                       (static_cast<QGraphicsRectItem*>(item))->setPen(currentPen);

                   } else if (selectedItem == espessura3) {
                       QPen currentPen = (static_cast<QGraphicsRectItem*>(item))->pen();

                       currentPen.setWidth(4);
                       (static_cast<QGraphicsRectItem*>(item))->setPen(currentPen);

                   } else if (selectedItem == corPreen) {
                       QColor cor = QColorDialog::getColor();

                       if (cor.isValid()) {

                           QPen currentPen = (static_cast<QGraphicsRectItem*>(item))->pen();
                           QBrush b = currentPen.brush();
                           b.setColor(cor);
                           b.setStyle(Qt::SolidPattern);

                           (static_cast<QGraphicsRectItem*>(item))->setBrush(b);
                       }

                   } else if (selectedItem == estiloPadrao1) {
                       QBrush b = (static_cast<QGraphicsRectItem*>(item))->brush();

                       b.setStyle(Qt::HorPattern);
                       (static_cast<QGraphicsRectItem*>(item))->setBrush(b);

                   } else if (selectedItem == estiloPadrao2) {
                       QBrush b = (static_cast<QGraphicsRectItem*>(item))->brush();

                       b.setStyle(Qt::VerPattern);
                       (static_cast<QGraphicsRectItem*>(item))->setBrush(b);

                   } else if (selectedItem == estiloPadrao3) {
                       QBrush b = (static_cast<QGraphicsRectItem*>(item))->brush();

                       b.setStyle(Qt::CrossPattern);
                       (static_cast<QGraphicsRectItem*>(item))->setBrush(b);
                   }
               }

           } else  if (tipoItem == MainWindow::Texto) {
               preenchimento->setEnabled(false);
               menuLinha->setEnabled(false);
               menuEspessura->setEnabled(false);

               selectedItem = menu.exec(globalPos);
               if (selectedItem && selectedItem == corLinha) {
                   QColor cor = QColorDialog::getColor();
                   if (cor.isValid())
                       (static_cast<QGraphicsTextItem*>(item))->setDefaultTextColor(cor);
               }

           } else if (tipoItem == MainWindow::Circulo) {
               selectedItem = menu.exec(globalPos);

               if (selectedItem) {
                   if (selectedItem == corLinha) {
                       QColor cor = QColorDialog::getColor();

                       if (cor.isValid()) {
                           QPen currentPen = (static_cast<QGraphicsEllipseItem*>(item))->pen();
                           currentPen.setColor(cor);
                           (static_cast<QGraphicsEllipseItem*>(item))->setPen(currentPen);
                       }
                   } else if (selectedItem == estilo1) {
                       QPen currentPen = (static_cast<QGraphicsEllipseItem*>(item))->pen();

                       currentPen.setStyle(Qt::DashLine);
                       (static_cast<QGraphicsEllipseItem*>(item))->setPen(currentPen);

                   } else if (selectedItem == estilo2) {
                       QPen currentPen = (static_cast<QGraphicsEllipseItem*>(item))->pen();

                       currentPen.setStyle(Qt::DashDotLine);
                       (static_cast<QGraphicsEllipseItem*>(item))->setPen(currentPen);

                   } else if (selectedItem == estilo3) {
                       QPen currentPen = (static_cast<QGraphicsEllipseItem*>(item))->pen();

                       currentPen.setStyle(Qt::DashDotLine);
                       (static_cast<QGraphicsEllipseItem*>(item))->setPen(currentPen);

                   } else if (selectedItem == espessura1) {
                       QPen currentPen = (static_cast<QGraphicsEllipseItem*>(item))->pen();

                       currentPen.setWidth(2);
                       (static_cast<QGraphicsEllipseItem*>(item))->setPen(currentPen);

                   } else if (selectedItem == espessura2) {
                       QPen currentPen = (static_cast<QGraphicsEllipseItem*>(item))->pen();

                       currentPen.setWidth(3);
                       (static_cast<QGraphicsEllipseItem*>(item))->setPen(currentPen);

                   } else if (selectedItem == espessura3) {
                       QPen currentPen = (static_cast<QGraphicsEllipseItem*>(item))->pen();

                       currentPen.setWidth(4);
                       (static_cast<QGraphicsEllipseItem*>(item))->setPen(currentPen);

                   } else if (selectedItem == corPreen) {
                       QColor cor = QColorDialog::getColor();

                       if (cor.isValid()) {

                           QPen currentPen = (static_cast<QGraphicsEllipseItem*>(item))->pen();
                           QBrush b = currentPen.brush();
                           b.setColor(cor);
                           b.setStyle(Qt::SolidPattern);

                           (static_cast<QGraphicsEllipseItem*>(item))->setBrush(b);
                       }

                   } else if (selectedItem == estiloPadrao1) {
                       QBrush b = (static_cast<QGraphicsEllipseItem*>(item))->brush();

                       b.setStyle(Qt::HorPattern);
                       (static_cast<QGraphicsEllipseItem*>(item))->setBrush(b);

                   } else if (selectedItem == estiloPadrao2) {
                       QBrush b = (static_cast<QGraphicsEllipseItem*>(item))->brush();

                       b.setStyle(Qt::VerPattern);
                       (static_cast<QGraphicsEllipseItem*>(item))->setBrush(b);

                   } else if (selectedItem == estiloPadrao3) {
                       QBrush b = (static_cast<QGraphicsEllipseItem*>(item))->brush();

                       b.setStyle(Qt::CrossPattern);
                       (static_cast<QGraphicsEllipseItem*>(item))->setBrush(b);
                   }
               }
           }
       }
    }
}


void MainWindow::on_actionTrazer_para_frente_triggered()
{
    // envia para TRAS todos os itens selecionados
    foreach (QGraphicsItem *item, graphicsScene->selectedItems())
        item->setZValue(1);
}

void MainWindow::on_actionEnviar_para_tr_s_triggered()
{
    // envia para TRAS todos os itens selecionados
    foreach (QGraphicsItem *item, graphicsScene->selectedItems())
        item->setZValue(-1);
}
