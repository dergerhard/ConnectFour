#ifndef CONNECTFOURGAME_H
#define CONNECTFOURGAME_H

#include <QObject>
#include <QColor>
#include <iostream>

#include "collections.h"
#include "connectfourwidget.h"
#include "graphicobjects.h"

#include <QMatrix4x4>
#include <QVector3D>

#include <qmath.h>
#include <iostream>

#include "graphicpart.h"
#include "settings.h"


using namespace Collections;


class ConnectFourWidget;


enum Direction { PLUSX, MINUSX, PLUSZ, MINUSZ };

/**
 * @brief This class is responsible for making the graphical representation of the ConnectFour Game.
 *
 */
class ConnectFourGame : public QObject
{
    Q_OBJECT

public:
    ConnectFourGame(ConnectFourWidget *parent, int x=7, int y=6, int z=1);
    ~ConnectFourGame();

    void draw() const;

    void addCubeTest();
    void setCube(int x, int y, int z, Player p);
    void putCubeOnTopOfSelector(Player p);
    void toggleDetails();
    void moveSelector(Direction d);
    void makeComputerMove();

    Player getTurn();
    Player isThereAWinner();
    bool isBoardFull();

    const int                   DIMX, DIMY, DIMZ;
    const int                   WIN=4;

private:
    bool showDetails = false;
    void buildGeometry();
    inline void countUp(int &playerAwin, int &playerBwin, int x, int y, int z);
    Player checkPlaneInDirection( char iIs, char jIs, char kIs);
    Player checkDiagonalInPlane(char xIs, char yIs, char zIs);

    ConnectFourWidget           *parent;
    qreal                       animationProgress;
    QList<Shape*>               parts;
    Geometry                    *geom;

    QTimer                      *timer = NULL;
    int                         currentAnimationCube[3] = {-1,-1,-1}; //used to control the fly-in of a cube

    QTimer                      *donutTimer;
    Torus                      *selectionDonut; //rushing down
    Torus                      *selectionHelper; //showing the way
    qreal                       wobble1=0.0, wobble2=0.0, wobbleMax=0.0;
    int                         wobbleTime, wobblePause, wobbleTimeStep;

    int                         selPosX, selPosZ;

    ArrayDList<PlayerCube*>    *field;
    ArrayDList<Player>       *logicField;
    Player                   turn=PlayerA; // whose turn is it  PlayerA is always the local one

public slots:
    void animCubeFlyIn();
    void animSelector();

};



#endif // CONNECTFOURGAME_H
