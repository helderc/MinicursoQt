#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QKeyEvent>
#include <QInputDialog>

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent = 0);

    enum Mode { MoveItem, InsLine, InsCirc, InsQuad, InsText, InsDraw };

    void setMode(Mode m);
    void setColor(QColor cor) { color = cor; }

private:

    Mode myMode;
    QGraphicsLineItem *line;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rect;
    QGraphicsTextItem *text;
    QPointF *lastPoint;
    QColor color;

    QGraphicsLineItem *draw;
    QGraphicsItemGroup *group;

    bool fixo;

    qreal xFixo;
    qreal yFixo;

    char movimentoAnteriorQuadrado;

public slots:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void novoDesenho();

};

#endif // GRAPHICSSCENE_H
