#include "glwidget.h"

GlWidget::GlWidget(QWidget *parent) :
    QGLWidget(parent)
{
    fatorZ = 1.0;
}

GlWidget::~GlWidget()
{

}

void GlWidget::initializeGL()
{
    // Para um melhor calculo de perspectiva
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // Define a cor de fundo da janela de visualização como branca
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glLineWidth(2.0);

    //gluOrtho2D(-01.0, 10.0, -01.01, 10.0);
}

void GlWidget::paintGL()
{
    // Limpa a janela de visualização com a cor de fundo definida previamente
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Define a viewport
    glViewport(0, 0, this->width(), this->height());
    glColor3f(1.0f, 0.0f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT);



    glPushMatrix();
        // Especifica sistema de coordenadas do modelo de visualização
        glMatrixMode(GL_MODELVIEW);
                // Inicializa sistema de coordenadas do modelo de visualização
        glLoadIdentity();
       // rotacao em torno do eixo X
       glRotatef(fatorZ, 1, 0, 0);


    // boxes
    glColor3f(1.0, 0.0, 1.1);
    glRectf(1.0, 1.0, 5.0, 5.0);

    glColor3f(0.0, 1.0, 0.0);
    glRectf(6.0, 1.0, 8.0, 5.0);

    glColor3f(0.0, 0.0, 1.0);
    glRectf(1.0, 6.0, 5.0, 8.0);

    glColor3f(0.0, 1.0, 1.0);
    glRectf(6.0, 6.0, 8.0, 8.0);

    // box lines
    glColor3f(0.4, 0.4, 1.0);
    glBegin(GL_LINES);
        // left
        glVertex2f(0.5, 0.5);
        glVertex2f(0.5, 8.5);

        // top
        glVertex2f(0.5, 8.5);
        glVertex2f(8.5, 8.5);

        // right
        glVertex2f(8.5, 8.5);
        glVertex2f(8.5, 0.5);

        // bottom
        glVertex2f(8.5, 0.5);
        glVertex2f(0.5, 0.5);
    glEnd();

    // coordinate system
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
        // horizontal
        glVertex2f(-10.0, 0.0);
        glVertex2f(10.0, 0.0);

        // vertical
        glVertex2f(0.0, 10.0);
        glVertex2f(0.0, -10.0);
    glEnd();

    glPopMatrix();

    glFlush();
}

void GlWidget::resizeGL(int w, int h)
{
}

void GlWidget::zoom(int i)
{
    fatorZ = (GLfloat)(i/100);

    qDebug() << i;
}

