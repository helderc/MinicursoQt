#include "graphicsscene.h"

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
    myMode = GraphicsScene::MoveItem;
}

void GraphicsScene::setMode(Mode m)
{
    myMode = m;
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Clicks com o botao direito sao ignorados
    if (event->button() == Qt::RightButton)
        return;

    switch (myMode) {
        // Desenha um linha
        case InsLine:
            line = new QGraphicsLineItem(QLineF(event->scenePos(),
                                                event->scenePos()));
            line->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
            line->setPen(QPen(Qt::black, 2));
            addItem(line);
            break;

        // Desenha um circulo
        case InsCirc:
            ellipse = new QGraphicsEllipseItem(event->scenePos().x(),
                                               event->scenePos().y(),
                                               0, 0);
            ellipse->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
            ellipse->setPen(QPen(Qt::black, 2));
            addItem(ellipse);
            break;

        // Para mover eh necessario propagar o evento
        case MoveItem:
            QGraphicsScene::mousePressEvent(event);
            break;
    }
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // Clicks com o botao direito sao ignorados
    if (event->button() == Qt::RightButton)
        return;

    // Linha
    if (myMode == InsLine && line) {
        QLineF newLine(line->line().p1(), event->scenePos());
        line->setLine(newLine);

    // Circulo/Elipse
    } else if (myMode == InsCirc && ellipse) {
        ellipse->setRect(ellipse->rect().x(), ellipse->rect().y(),
                         event->scenePos().x() - ellipse->rect().x(),
                         event->scenePos().y() - ellipse->rect().y());

    // Movimento de Objetos
    } else if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(event);
    }

}

void GraphicsScene::keyPressEvent(QKeyEvent *event)
{
    // exclui os elementos que estão selecionados se o usuário pressionar DELETE
    if (event->key() == Qt::Key_Delete) {
        // pega todos os items selecionados e exclui um-a-um
        foreach (QGraphicsItem *item, selectedItems())
            removeItem(item);
    }
}


void GraphicsScene::novoDesenho()
{
    // TODO: Verificar se irá salvar o documento corrente

    // exclui todos os items da cena
    foreach (QGraphicsItem *item, items())
        removeItem(item);
}
