#include "graphicsscene.h"

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
    // inicializa os elementos com 0
    line = 0;
    ellipse = 0;
    rect = 0;
    text = 0;
    lastPoint = 0;

    color = QColor(Qt::black);
    myMode = GraphicsScene::MoveItem;

    fixo = false;
}

void GraphicsScene::setMode(Mode m)
{
    myMode = m;
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    bool ok;
    QString texto;

    // Clicks com o botao direito sao ignorados
    if (event->button() == Qt::RightButton)
        return;

    switch (myMode) {
        // Desenha um linha
        case InsLine:
            line = new QGraphicsLineItem(QLineF(event->scenePos(),
                                                event->scenePos()));
            line->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
            line->setPen(QPen(color, 2));
            addItem(line);

            break;
        // Desenha um circulo
        case InsCirc:
            ellipse = new QGraphicsEllipseItem(event->scenePos().x(),
                                               event->scenePos().y(),
                                               0, 0);
            ellipse->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
            ellipse->setPen(QPen(color, 2));
            addItem(ellipse);

            break;

        // Desenha um retangulo
        case InsQuad:
            movimentoAnteriorQuadrado = 1;
            rect = new QGraphicsRectItem(event->scenePos().x(),
                                         event->scenePos().y(),
                                         0, 0);
            rect->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
            rect->setPen(QPen(color, 2));
            addItem(rect);
            fixo = false;
            break;

        // Insere um texto
        case InsText:
            texto = QInputDialog::getText(0, "Título", "Insira seu texto", QLineEdit::Normal,
                                                  "", &ok);
            if (ok && !texto.isEmpty()) {
                text = new QGraphicsTextItem(texto);
                text->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
                text->setPos(event->scenePos());
                addItem(text);
            }
            break;

        // Lapis: desenho livre
        case InsDraw:
            group = new QGraphicsItemGroup();
            group->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
            addItem(group);
            lastPoint = new QPointF(event->scenePos());
            draw = new QGraphicsLineItem(QLineF(*lastPoint, *lastPoint));
            group->addToGroup(draw);

            break;

        // Para mover eh necessario propagar o evento
        case MoveItem:
            QGraphicsScene::mousePressEvent(event);
            break;
    }
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    line = 0;
    ellipse = 0;
    rect = 0;
    text = 0;
    lastPoint = 0;

    QGraphicsScene::mouseReleaseEvent(event);
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

    // Quadrado
    } else if (myMode == InsQuad && rect) {
        qreal rectX = rect->rect().x();
        qreal rectY = rect->rect().y();

        QGraphicsRectItem* rem = rect;

        if (movimentoAnteriorQuadrado == 1) {
            if (rect->rect().x()+rect->rect().width() < event->scenePos().x()) {
                if (rect->rect().y() <= event->scenePos().y())
                    movimentoAnteriorQuadrado = 2;
                else
                    movimentoAnteriorQuadrado = 4;
            } else if (rect->rect().y() > event->scenePos().y())
                movimentoAnteriorQuadrado = 3;

        } else if (movimentoAnteriorQuadrado == 2) {
            if (rect->rect().x() > event->scenePos().x()) {
                if (rect->rect().y() <= event->scenePos().y())
                    movimentoAnteriorQuadrado = 1;
                else
                    movimentoAnteriorQuadrado = 3;
            } else if (rect->rect().y() > event->scenePos().y())
                movimentoAnteriorQuadrado = 4;

        } else if (movimentoAnteriorQuadrado == 3) {
            if (rect->rect().x()+rect->rect().width() < event->scenePos().x()) {
                if (rect->rect().y()+rect->rect().height() <= event->scenePos().y())
                    movimentoAnteriorQuadrado = 2;
                else
                    movimentoAnteriorQuadrado = 4;
            } else if (rect->rect().y()+rect->rect().height() < event->scenePos().y())
                movimentoAnteriorQuadrado = 1;

        } else if (movimentoAnteriorQuadrado == 4) {
            if (rect->rect().x() > event->scenePos().x()) {
                if (rect->rect().y()+rect->rect().height() <= event->scenePos().y())
                    movimentoAnteriorQuadrado = 1;
                else
                    movimentoAnteriorQuadrado = 3;
            } else if (rect->rect().y()+rect->rect().height() < event->scenePos().y())
                movimentoAnteriorQuadrado = 2;
        }

        if (movimentoAnteriorQuadrado == 1) {
            rect = new QGraphicsRectItem(event->scenePos().x(), rectY,
                                         rect->rect().x() > event->scenePos().x()
                                         ?
                                         rect->rect().width() + abs(rect->rect().x() - event->scenePos().x())
                                         :
                                         rect->rect().width() - abs(rect->rect().x() - event->scenePos().x()) ,
                                         abs(event->scenePos().y() - rectY));
            rect->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
            addItem(rect);
            delete rem;
            movimentoAnteriorQuadrado = 1;
        } else if (movimentoAnteriorQuadrado == 2) {

            rect = new QGraphicsRectItem(rect->rect().x(), rect->rect().y(),
                                         rect->rect().x() < event->scenePos().x()
                                         ?
                                          abs(event->scenePos().x() - rect->rect().x())
                                         :
                                         abs(event->scenePos().x() - rect->rect().x())
                                         ,
                                         abs(event->scenePos().y() - rectY));
            rect->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
            addItem(rect);
            delete rem;
            movimentoAnteriorQuadrado = 2;
        } else if (movimentoAnteriorQuadrado == 3) {

            if (!fixo) {
                xFixo = rectX;
                yFixo = rectY;
                fixo = true;
            }

            rect = new QGraphicsRectItem(event->scenePos().x(), event->scenePos().y(),
                                         event->scenePos().x() < rect->rect().x()
                                         ?
                                         rect->rect().width() + abs(event->scenePos().x() - rect->rect().x())
                                         :
                                         rect->rect().width() - abs(event->scenePos().x() - rect->rect().x()),
                                         abs(yFixo - event->scenePos().y()));
            rect->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
            addItem(rect);
            delete rem;
            movimentoAnteriorQuadrado = 3;

        } else if (movimentoAnteriorQuadrado == 4) {
            if (!fixo) {
                xFixo = rectX;
                yFixo = rectY;
                fixo = true;
            }
            rect = new QGraphicsRectItem(xFixo, event->scenePos().y(),                                         
                                         abs(event->scenePos().x() - xFixo),
                                         abs(yFixo - event->scenePos().y()));
            rect->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
            addItem(rect);
            delete rem;
            movimentoAnteriorQuadrado = 4;
        }

    // Lapis
    } else if (myMode == InsDraw && lastPoint) {
        QPointF ponto(event->scenePos());

        draw = new QGraphicsLineItem(QLineF(*lastPoint, ponto));
        group->addToGroup(draw);

        *lastPoint = ponto;

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
    // exclui todos os items da cena
    foreach (QGraphicsItem *item, items())
        removeItem(item);
}
