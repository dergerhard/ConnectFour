#include "graphicobjects.h"


Shape::Shape()
{

}

QVector<QVector3D> Shape::extrude(const QVector<QVector3D> &verts, qreal depth)
{
    QVector<QVector3D> extr = verts;
    for (int v = 0; v < extr.count(); ++v)
        extr[v].setZ(extr[v].z() - depth);
    return extr;
}

void Shape::translate(const QVector3D &t)
{
    for (int i = 0; i < parts.count(); ++i)
        parts[i]->translate(t);
}

void Shape::rotate(qreal deg, QVector3D axis)
{
    for (int i = 0; i < parts.count(); ++i)
        parts[i]->rotate(deg, axis);
}

void Shape::draw()
{
    foreach (Patch *p, parts)
        p->draw();
}

void Shape::setColor(QColor c)
{
    foreach (Patch *p, parts)
    {

        p->faceColor[0] = c.redF();
        p->faceColor[1] = c.greenF();
        p->faceColor[2] = c.blackF();
        p->faceColor[3] = c.alphaF();
    }
}


/**
 * @brief Set the alpha channel.
 *
 * @param t must be between 0 and 1
 */
void Shape::setTransparency(qreal t)
{
    foreach (Patch *p, parts)
        p->faceColor[3] = t;

}


Cube::Cube(Geometry *g, qreal width, qreal height, qreal depth)
{
    enum { bl, br, tr, tl };
    Patch *fb = new Patch(g);
    fb->setSmoothing(Patch::Faceted);

    // front face
    QVector<QVector3D> r(4);
    r[br].setX(width);
    r[tr].setX(width);
    r[tr].setY(height);
    r[tl].setY(height);
    QVector3D adjToCenter(-width / 2.0, -height / 2.0, depth / 2.0);
    for (int i = 0; i < 4; ++i)
        r[i] += adjToCenter;
    fb->addQuad(r[bl], r[br], r[tr], r[tl]);

    // back face
    QVector<QVector3D> s = extrude(r, depth);
    fb->addQuad(s[tl], s[tr], s[br], s[bl]);

    // side faces
    Patch *sides = new Patch(g);
    sides->setSmoothing(Patch::Faceted);
    sides->addQuad(s[bl], s[br], r[br], r[bl]);
    sides->addQuad(s[br], s[tr], r[tr], r[br]);
    sides->addQuad(s[tr], s[tl], r[tl], r[tr]);
    sides->addQuad(s[tl], s[bl], r[bl], r[tl]);

    parts << fb << sides;
}

Cube::Cube()
{

}





/**
 * @brief Default constructor, called when initializing game field. Nothing is constructed here. The cube will be constructed, when the player disc is set.
 *
 */
PlayerCube::PlayerCube()
{
    state = NotSet;
    i1=i2=i3=0;
}

/*
void PlayerCube::flyin()
{
    //...
}
*/

/**
 * @brief Sets the state of the cube to one of the players. This can't be undone (but isn't necessary neighter). If a player value other than PlayerA/PlayerB is set, nothing will happen.
 *
 * @param g Geometry to be drawn in
 * @param p Player state
 */
void PlayerCube::setState(Geometry *g, Player p, int i1, int i2, int i3)
{
    if ((p==PlayerA || p==PlayerB) && state==NotSet)
    {
        state = p;

        qreal width, height, depth;
        width = height = depth = Settings::CubeSize;

        enum { bl, br, tr, tl };
        Patch *fb = new Patch(g);
        fb->setSmoothing(Patch::Smooth);

        // front face
        QVector<QVector3D> r(4);
        r[br].setX(width);
        r[tr].setX(width);
        r[tr].setY(height);
        r[tl].setY(height);
        QVector3D adjToCenter(-width / 2.0, -height / 2.0, depth / 2.0);
        for (int i = 0; i < 4; ++i)
            r[i] += adjToCenter;
        fb->addQuad(r[bl], r[br], r[tr], r[tl]);

        // back face
        QVector<QVector3D> s = extrude(r, depth);
        fb->addQuad(s[tl], s[tr], s[br], s[bl]);

        // side faces
        Patch *sides = new Patch(g);
        sides->setSmoothing(Patch::Faceted);
        sides->addQuad(s[bl], s[br], r[br], r[bl]);
        sides->addQuad(s[br], s[tr], r[tr], r[br]);
        sides->addQuad(s[tr], s[tl], r[tl], r[tr]);
        sides->addQuad(s[tl], s[bl], r[bl], r[tl]);

        parts << fb << sides;

        this->i1 = i1;
        this->i2 = i2;
        this->i3 = i3;

        qreal cs = Settings::CubeSize;
        qreal ef = Settings::EdgeFactor;

        //set position
        translate(QVector3D(i1*cs*ef, i2*cs*ef + cs*ef/2, i3*cs*ef));

        //set color
        if (p==PlayerA)
            setColor(QColor(255,0,0,240));
        else if (p==PlayerB)
            setColor(QColor(0,0,100,240));
        else setColor(QColor("grey"));

        //std::cout << "PlayerCube state: " << p << std:: endl;
        //QTimer *timer = new QTimer;

        //connect(timer, SIGNAL(timeout()), this, SLOT(flyin()));
    }

}

void PlayerCube::draw()
{
    if (state!=NotSet)
       Shape::draw();
}



/**
 * @brief Retreives the state of the field
 *
 * @return Player
 */
Player PlayerCube::getState()
{
    return state;
}

void PlayerCube::Print(std::ostream &out)
{
    out << "i1: " << i1 << " i2: " << i2 << " i3: " << i3 << " state: " << state;
}

std::ostream& operator<< (std::ostream &out, PlayerCube &obj)
{
    obj.Print(out);
    return out;
}


Torus::Torus(Geometry *g, qreal iRad, qreal oRad, qreal depth, int k)
{
    QVector<QVector3D> inside;
    QVector<QVector3D> outside;
    for (int i = 0; i < k; ++i) {
        qreal angle = (i * 2 * M_PI) / k;
        inside << QVector3D(iRad * qSin(angle), iRad * qCos(angle), depth / 2.0);
        outside << QVector3D(oRad * qSin(angle), oRad * qCos(angle), depth / 2.0);
    }
    inside << QVector3D(0.0, iRad, 0.0);
    outside << QVector3D(0.0, oRad, 0.0);
    QVector<QVector3D> in_back = extrude(inside, depth);
    QVector<QVector3D> out_back = extrude(outside, depth);

    // Create front, back and sides as separate patches so that smooth normals
    // are generated for the curving sides, but a faceted edge is created between
    // sides and front/back
    Patch *front = new Patch(g);
    for (int i = 0; i < k; ++i)
        front->addQuad(outside[i], inside[i],
                       inside[(i + 1) % k], outside[(i + 1) % k]);
    Patch *back = new Patch(g);
    for (int i = 0; i < k; ++i)
        back->addQuad(in_back[i], out_back[i],
                      out_back[(i + 1) % k], in_back[(i + 1) % k]);
    Patch *is = new Patch(g);
    for (int i = 0; i < k; ++i)
        is->addQuad(in_back[i], in_back[(i + 1) % k],
                    inside[(i + 1) % k], inside[i]);
    Patch *os = new Patch(g);
    for (int i = 0; i < k; ++i)
        os->addQuad(out_back[(i + 1) % k], out_back[i],
                    outside[i], outside[(i + 1) % k]);
    parts << front << back << is << os;
}

