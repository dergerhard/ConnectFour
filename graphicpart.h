#ifndef GRAPHICPART_H
#define GRAPHICPART_H

#include <QMatrix4x4>
#include <QVector3D>
#include <QGLWidget>
#include <iostream>


static void qSetColor(float colorVec[], QColor c);
static void qMultMatrix(const QMatrix4x4 &mat);

/**
 * @brief  The geometry is divided into a list of parts which may be rendered in different ways.
 * The data itself is contained in a Geometry structure that includes the vertices, their lighting
 * normals and index values which point into the vertices, grouping them into faces.
 *
 */
struct Geometry
{
    QVector<GLushort> faces;
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    void appendSmooth(const QVector3D &a, const QVector3D &n, int from);
    void appendFaceted(const QVector3D &a, const QVector3D &n);
    void finalize();
    void loadArrays() const;
};


class Patch
{
public:
    enum Smoothing { Faceted, Smooth };
    Patch(Geometry *);
    void setSmoothing(Smoothing s) { sm = s; }
    void translate(const QVector3D &t);
    void rotate(qreal deg, QVector3D axis);
    void draw() const;
    void addTri(const QVector3D &a, const QVector3D &b, const QVector3D &c, const QVector3D &n);
    void addQuad(const QVector3D &a, const QVector3D &b,  const QVector3D &c, const QVector3D &d);

    GLushort start;
    GLushort count;
    GLushort initv;

    GLfloat faceColor[4];
    QMatrix4x4 mat;
    Smoothing sm;
    Geometry *geom;
};



#endif // GRAPHICPART_H
