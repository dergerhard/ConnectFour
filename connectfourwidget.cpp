/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "connectfourwidget.h"



#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif




ConnectFourWidget::ConnectFourWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    game = 0;
    xRot = 0;
    yRot = 0;
    zRot = 0;
    scale = 1.0f;
}


ConnectFourWidget::~ConnectFourWidget()
{
}


void ConnectFourWidget::setCube(int x, int y, int z, Player p)
{
    game->setCube(x,y,z,p);
    //resizeGL(2000, 2000);
    updateGL();
}


QSize ConnectFourWidget::minimumSizeHint() const
{
    return QSize(800, 600);
}


QSize ConnectFourWidget::sizeHint() const
{
    return QSize(800, 600);
}



static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}



void ConnectFourWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}



void ConnectFourWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}


void ConnectFourWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}


void ConnectFourWidget::initializeGL()
{
    //QPalette obj;
    //qglClearColor(obj.window().color());
    qglClearColor(QColor("black"));


    game = new ConnectFourGame(this, 7,8,9);

    //alpha channel
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    //glShadeModel(GL_FLAT);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);

    //glEnable ( GL_COLOR_MATERIAL ) ;
    //glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;

    static GLfloat lightPosition[4] = { 100.5, 150.0, 170.0, 100.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    //glLightf(GL_LIGHT0, GL_DIFFUSE, );
    //glLightf();
}


void ConnectFourWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    glScalef(scale, scale, scale);
    game->draw();
}


void ConnectFourWidget::resizeGL(int width, int height)
{
    glViewport(0,0,width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#else
    glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}


void ConnectFourWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:  game->moveSelector(MINUSX); break;
    case Qt::Key_Right: game->moveSelector(PLUSX); break;
    case Qt::Key_Up: game->moveSelector(MINUSZ); break;
    case Qt::Key_Down: game->moveSelector(PLUSZ); break;
    case Qt::Key_Space:
        if (game->getTurn()==PlayerA)
            game->putCubeOnTopOfSelector(PlayerA);
        else game->putCubeOnTopOfSelector(PlayerB); break;
    case Qt::Key_Shift: game->toggleDetails(); break;
    case Qt::Key_Plus: scale = scale*1.2f; updateGL(); break;
    case Qt::Key_Minus: scale = scale/1.2f; updateGL(); break;

    }
    game->isThereAWinner();

}


void ConnectFourWidget::mousePressEvent(QMouseEvent *event)
{
    this->setFocus();
    lastPos = event->pos();
}



void ConnectFourWidget::wheelEvent(QWheelEvent *event)
{
    if (event->delta()>0)
        scale = scale*1.2f;
    else
        scale = scale/1.2f;
    updateGL();
}


void ConnectFourWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();
}


