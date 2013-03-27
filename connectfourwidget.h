#ifndef CONNECTFOURWIDGET_H
#define CONNECTFOURWIDGET_H

#include "connectfourgame.h"
#include "graphicobjects.h"

#include <QGLWidget>
#include <QtGui>
#include <QtOpenGL>
#include <math.h>


class ConnectFourGame;
enum Player;


class ConnectFourWidget : public QGLWidget
{
    Q_OBJECT

public:
    ConnectFourWidget(QWidget *parent = 0);
    ~ConnectFourWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void setCube(int x, int y, int z, Player p);

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    ConnectFourGame *game;
    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos;
    GLfloat scale; // for zooming
    QTimer *timer;
};



#endif
