#ifndef GRAPHICOBJECTS_H
#define GRAPHICOBJECTS_H

#include "graphicpart.h"
#include "settings.h"

#include <iostream>
#include <QTimer>
#include <QObject>
#include <math.h>
#include <qmath.h>

enum Player { NotSet=0, PlayerA=1, PlayerB=-1 };


class Shape
{
public:
    QVector<QVector3D> extrude(const QVector<QVector3D> &verts, qreal depth);
    void translate(const QVector3D &t);
    void rotate(qreal deg, QVector3D axis);
    void draw();
    void setColor(QColor c);
    void setTransparency(qreal t);
    QList<Patch*> parts;
protected:
    Shape();
};



class Cube : public Shape
{
public:
    Cube(Geometry *g, qreal width, qreal height, qreal depth);
    Cube();
};



class PlayerCube  : public Shape
{
    friend std::ostream& operator<< (std::ostream &out, PlayerCube &obj);
private:
    Player state;
    int i1,i2,i3; //not needed -debug
public:
    PlayerCube();
    void setState(Geometry *g, Player p, int i1, int i2, int i3);
    Player getState();
    void Print(std::ostream &out);
    void draw();
};



class Torus : public Shape
{
public:
    Torus(Geometry *g, qreal iRad, qreal oRad, qreal depth, int numSectors);
};



#endif // GRAPHICOBJECTS_H
