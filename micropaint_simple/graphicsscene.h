#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QKeyEvent>

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent = 0);

    enum Mode { MoveItem, InsLine, InsCirc };

    void setMode(Mode m);

private:

    Mode myMode;
    QGraphicsLineItem *line;
    QGraphicsEllipseItem *ellipse;

public slots:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void novoDesenho();

};

#endif // GRAPHICSSCENE_H
