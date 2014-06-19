#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QDebug>

class GlWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GlWidget(QWidget *parent = 0);
    ~GlWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL( int w, int h );

private:
    GLfloat fatorZ;

public slots:
    void zoom(int i);

    
};

#endif // GLWIDGET_H
