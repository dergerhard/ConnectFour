#include "connectfourgame.h"

static const qreal tee_height = 0.311126;
static const qreal cross_width = 0.25;
static const qreal bar_thickness = 0.113137;
static const qreal inside_diam = 0.20;
static const qreal outside_diam = 0.30;
static const qreal logo_depth = 0.10;
static const int num_divisions = 32;


ConnectFourGame::ConnectFourGame(QWidget *parent, const QString &playerName, const QString &gameName, int x, int y, int z, Player turn)
    : ConnectFourWidget(parent), geom(new Geometry()), playerName(playerName),
      gameName(gameName), DIMX(x), DIMY(y), DIMZ(z), turn(turn)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;
    scale = 1.0f;


    //make sure, the game has as z-dimension at least 1
    if (z<1)
        z=1;

    //initialize graphic and logic fields
    field = new ArrayDList<PlayerCube*>(x,y,z);
    logicField = new ArrayDList<Player>(x,y,z);

    for (int i=0; i<DIMX; i++)
        for (int j=0; j<DIMY; j++)
            for (int k=0; k<DIMZ; k++)
            {
                field->set(new PlayerCube(), i,j,k);
                logicField->set(NotSet, i,j,k);
            }

            //new ArrayDList<PlayerCube>(7,7,6);
    selPosX=0;
    selPosZ=0;
    buildGeometry();

    //save moveIDs for later identification with protocol
    //z*DIMX*DIMY + y*DIMX + x
    for (int x=0; x<DIMX; x++)
        for (int y=0; y<DIMY; y++)
            for (int z=0; z<DIMZ; z++)
                moveIDs.insert(z*DIMX*DIMY+y*DIMX+x, QVector3D(x,y,z));

}

ConnectFourGame::~ConnectFourGame()
{
    //todo!! delete field
    qDeleteAll(parts);
    delete geom;
}


QVector3D ConnectFourGame::getMoveFromID(int id, bool onlyIDNoCheck)
{
    if (moveIDs.contains(id))
    {
        QVector3D m = moveIDs[id];
        if (onlyIDNoCheck)
            return m;

        if (logicField->get(m.x(), m.y(), m.z())==NotSet)
            return m;
        return QVector3D(-2,-2,-2); //index correct, field already set
    }
    return QVector3D(-1,-1,-1); //index incorrect
}

int ConnectFourGame::getIDFromMove(const QVector3D &move)
{
    return (int)move.z()*DIMX*DIMY+move.y()*DIMX+move.x();
}

Player ConnectFourGame::getFieldState(const QVector3D &move)
{
    return logicField->get((int)move.x(),(int)move.y(),(int)move.z());
}

void ConnectFourGame::setState(bool setOpen)
{
    isOpen = setOpen;
}

void ConnectFourGame::buildGeometry()
{
    qreal edge = Sett::ings().getReal("3d/cubesize");
    qreal colw = Sett::ings().getReal("3d/columnwidth");
    qreal ef = Sett::ings().getReal("3d/edgefactor");

    int dim2 = DIMZ==0 ? 1 : DIMZ;

    //create bottom surface - different colored rectangles
    for (int i=0; i<DIMX; i++)
        for (int j=0; j<dim2; j++)
        {
            //x=2, y=3, z=4  --> surface is in x-z plane
            Cube *rect= new Cube(geom,  edge*ef, colw, edge*ef);

            rect->translate(QVector3D(i*edge*ef, 0, j*edge*ef));
            //rect->setColor(QColor(255/DIMX*i, 255/DIMY*j, 150));
            rect->setColor(QColor(0, 255/(DIMX*dim2)*(i*dim2+j), 0));
            parts << rect;
        }

    //Edge Columns - for orientation
    Cube *col= new Cube(geom, colw, edge*DIMY*ef, colw);
    col->translate(QVector3D(-edge*ef/2, edge*DIMY*ef/2, -edge*ef/2));
    col->setColor(QColor("black"));
    parts << col;

    col= new Cube(geom, colw, edge*DIMY*ef, colw);
    col->translate(QVector3D(edge*ef/2+edge*ef*(DIMX-1), edge*DIMY*ef/2, -edge*ef/2));
    col->setColor(QColor("black"));
    parts << col;

    col= new Cube(geom, colw, edge*DIMY*ef, colw);
    col->translate(QVector3D(-edge*ef/2, edge*DIMY*ef/2, edge*ef/2+edge*ef*(DIMZ-1)));
    col->setColor(QColor("black"));
    parts << col;

    col= new Cube(geom, colw, edge*DIMZ*ef, colw);
    col->translate(QVector3D(-edge*ef/2, edge*DIMY*ef, -edge*ef/2+edge*DIMZ*ef/2));
    col->rotate(90.0f, QVector3D(1.0f, 0.0f, 0.0f));
    col->setColor(QColor("black"));
    parts << col;

    col= new Cube(geom, colw, edge*DIMX*ef, colw);
    col->translate(QVector3D(-edge*ef/2+edge*DIMX*ef/2, edge*DIMY*ef, -edge*ef/2));
    col->rotate(90.0f, QVector3D(0.0f, 0.0f, 1.0f));
    col->setColor(QColor("black"));
    parts << col;



    //create selection helper
    selectionHelper = new Torus(geom,Sett::ings().getReal("3d/cubesize")*0.0005, Sett::ings().getReal("3d/cubesize")*0.3, edge*DIMY*ef, 100);
    selectionHelper->setColor(QColor(150,150,150,80));
    selectionHelper->translate(QVector3D(0, edge*DIMY*ef/2, 0));
    selectionHelper->rotate(90.0, QVector3D(1.0,0.0,0.0));
    parts << selectionHelper;

    //store selection position for keyboard navigation
    selPosX=0;
    selPosZ=0;

    //create selection donut
    selectionDonut = new Torus(geom,Sett::ings().getReal("3d/cubesize")*0.0005, Sett::ings().getReal("3d/cubesize")*0.32, Sett::ings().getReal("3d/cubesize")/5, 100);
    selectionDonut->translate(QVector3D(
                                  0,
                                  edge*DIMY*ef,
                                  0));
    selectionDonut->rotate(90.0, QVector3D(1.0,0.0,0.0));
    selectionDonut->setColor(QColor(240,140,40));
    parts << selectionDonut;

    //selectionDonut->rotate(10.0, QVector3D(1.0, 0.0, 0.0));
    wobbleTime = Sett::ings().getReal("3d/hanimtime");
    wobblePause = Sett::ings().getReal("3d/hanimpause");
    wobbleTimeStep = Sett::ings().getReal("3d/hanimstep");
    donutTimer = new QTimer;
    donutTimer->setInterval(wobbleTimeStep);
    connect(donutTimer, SIGNAL(timeout()), this, SLOT(animSelector()));
    donutTimer->start();
    geom->finalize();
}


void ConnectFourGame::draw()
{
    geom->loadArrays();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    //draw board basics
    for (int i = 0; i < parts.count(); ++i)
        parts[i]->draw();

    //draw all PlayerCubes
    for (int i=0; i<DIMX; i++)
        for (int j=0; j<DIMY; j++)
            for (int k=0; k<DIMZ; k++)
                if (field->get(i,j,k)!=0)
                    field->get(i,j,k)->draw();


    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    //std::cout << "draw()" << std::endl;
}


void ConnectFourGame::animCubeFlyIn()
{
    //check if already finished
    if (animationProgress < DIMY*2*Sett::ings().getReal("3d/cubesize")*Sett::ings().getReal("3d/edgefactor"))
    {
        PlayerCube *gp = field->get(currentAnimationCube[0],currentAnimationCube[1],currentAnimationCube[2]);
        //move down
        qreal moveLength = Sett::ings().getReal("3d/cubesize")/5;
        gp->translate(QVector3D(0, -moveLength, 0));
        //save again (because arraydlist cant hold pointers)
        //field->set(gp,currentAnimationCube[0],currentAnimationCube[1],currentAnimationCube[2]);
        updateGL();
        animationProgress += moveLength;
    }
    else
    {
        disconnect(this, SLOT(animCubeFlyIn()));
        delete timer;
        timer = NULL;
        currentAnimationCube[0]=-1;
        currentAnimationCube[1]=-1;
        currentAnimationCube[2]=-1;
    }
}


void ConnectFourGame::animSelector()
{
    qreal runs = (wobbleTime+wobblePause)/wobbleTimeStep;    //til cycle is finishe
    qreal moveRuns = wobbleTime/wobbleTimeStep;                        //number of runs to move it

    qreal step = Sett::ings().getReal("3d/cubesize")*Sett::ings().getReal("3d/edgefactor")*DIMY/moveRuns;

    if (wobble1==moveRuns)
    {
        selectionDonut->translate(QVector3D(0.0, 0.0, - step*moveRuns));
        selectionDonut->setColor(QColor(240,140,40, 0));
    }

    if (wobble1<moveRuns)
    {
        selectionDonut->setColor(QColor(240,140,40));
        selectionDonut->translate(QVector3D(0.0, 0.0, step));
    }

    if (wobble1>=runs) //cycle finished
    {
        wobble1 = 0;
        selectionDonut->setColor(QColor(240,140,40));
    }
    else wobble1 +=1;


    updateGL();
}



void ConnectFourGame::moveSelector(Direction d)
{
    qreal x=0.0, y=0.0, z=0.0;
    qreal size = Sett::ings().getReal("3d/cubesize");
    qreal ef = Sett::ings().getReal("3d/edgefactor");

    switch (d)
    {
        case PLUSX:
        if (selPosX<(DIMX-1))
        {
            x = size*ef;
            selPosX++;
        }
        break;

        case MINUSX:
        if (selPosX>0)
        {
            x = -size*ef;
            selPosX--;
        }
        break;

        case PLUSZ:
        if (selPosZ<(DIMZ-1))
        {
            y = size*ef;
            selPosZ++;
        }
        break;

        case MINUSZ:
        if (selPosZ>0)
        {
            y = - size*ef;
            selPosZ--;
        }
        break;
    }

    /*Explanation on PLUSZ and MINUSZ setting y:
     *  SelectionHelper and selectionDonut were rotated by 90 dagrees. When doing that, the coordinate-system is rotated with them.
     *  Therefor y must be used. Tried to correct it, but couldn't find a way to do it - thus the workaround.
     */
    selectionDonut->translate(QVector3D(x,y,z));
    selectionHelper->translate(QVector3D(x,y,z));
    toggleDetails();toggleDetails();
}





Player ConnectFourGame::getTurn()
{
    return turn;
}

void ConnectFourGame::setTurn(Player p)
{
    turn = p;
}


QVector3D ConnectFourGame::putCubeOnTopOfSelector(Player state)
{

    //#if (currentAnimationCube[0]<0)
    //#{
        //find the vertical index (in wich height will the cube be placed)
        int y=-1;
        for (int i=DIMY-1;i>=0; --i)
        {
            if  (field->get(selPosX,i,selPosZ)->getState()==NotSet)
                y=i;
        }

        //if not full...
        if (y>=0)
        {
            setCube(selPosX, y, selPosZ, state);
            return QVector3D(selPosX, y, selPosZ);
        }
        else return QVector3D(-1,-1,-1);
    //#}
    return QVector3D(-1,-1,-1);
}


bool ConnectFourGame::syncGameBoard(const QList<int> &syncStates)
{
    //could not use QMap... QVector3D is not sortable (no > <)
    QList<QVector3D> moveL;
    QList<int> stateL;

    for (int x=0; x<DIMX; x++)
        for (int y=0; y<DIMY; y++)
            for (int z=0; z<DIMZ; z++)
            {
                if (syncStates.size()>(z*DIMX*DIMY + y*DIMX + x))
                {
                        if (syncStates.at(z*DIMX*DIMY + y*DIMX + x)!=logicField->get(x,y,z))
                        {
                            moveL.append(QVector3D(x,y,z));
                            stateL.append(syncStates.at(z*DIMX*DIMY + y*DIMX + x));
                        }
                }
            }

    //no change, but ther should be change
    if (moveL.size()==0)
        return true;    //neverteless return true--> is just the confirmation

    int t = turn;
    if (moveL.size()==1 && stateL.at(0)==t)
    {
        setCube((int)moveL.at(0).x(), (int)moveL.at(0).y(), (int)moveL.at(0).z(), turn);
        return true;
    }
    return false;
}

void ConnectFourGame::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:  moveSelector(MINUSX); break;
    case Qt::Key_Right: moveSelector(PLUSX); break;
    case Qt::Key_Up: moveSelector(MINUSZ); break;
    case Qt::Key_Down: moveSelector(PLUSZ); break;
    case Qt::Key_Space:
        if (getTurn()==PlayerA)
            putCubeOnTopOfSelector(PlayerA);
        break;
    case Qt::Key_Shift: toggleDetails(); break;
    case Qt::Key_Plus: scale = scale*1.2f; break;
    case Qt::Key_Minus: scale = scale/1.2f; break;
    }
    updateGL();
}

void ConnectFourGame::setCube(int x, int y, int z, Player p)
{
    //wait for animation to finish
    //#while (currentAnimationCube[0]>=0)
    //#    usleep(20);

    //#if (currentAnimationCube[0]<0 && field->get(x,y,z)->getState()==NotSet) //if <0 --> animation locked
    if (field->get(x,y,z)->getState()==NotSet)
    {
        PlayerCube *gp = field->get(x,y,z);
        //activate
        gp->setState(geom, p, x, y, z);
        logicField->set(p, x,y,z);
        //reset to top (twice the height of the game)
        //#gp->translate(QVector3D(0, DIMY*2*Sett::ings().getReal("3d/cubesize")*Sett::ings().getReal("3d/edgefactor"), 0));


        //#
        /*
        if (timer!=NULL)
            cout << "timer is NOT NULL" << endl;

        if (timer==NULL)
        {
            timer = new QTimer();
            timer->setInterval(10);
            //save current indices - and lock
            currentAnimationCube[0]=x;
            currentAnimationCube[1]=y;
            currentAnimationCube[2]=z;

            animationProgress=0;
            //connect to fly-in-method
            connect(timer, SIGNAL(timeout()), this, SLOT(animCubeFlyIn()));
            timer->start();
        }*/

        //change turn
        if (turn==PlayerA)
        {
            turn = PlayerB;
            emit localPlayerMoved(QVector3D(selPosX, y, selPosZ));
        }
        else
        {
            turn = PlayerA;
            emit netPlayerMoved(QVector3D(selPosX, y, selPosZ));
        }


    }

}



/**
 * @brief turn on/off the detail view
 *
 */
void ConnectFourGame::toggleDetails()
{
    //selPosX selPosZ
    for (int x=0; x<DIMX; x++)
        for (int y=0; y<DIMY; y++)
            for (int z=0; z<DIMZ; z++)
            {
                if (showDetails==false)
                {
                    if (x!=selPosX)
                        field->get(x, y, z)->setTransparency(0.2f);
                }
                else
                {
                    field->get(x,y,z)->setTransparency(1.0f);
                }
            }
    showDetails = !showDetails;
}



bool ConnectFourGame::isBoardFull()
{
    for (int x=0; x<DIMX; x++)
        for (int y=0; y<DIMY; y++)
            for (int z=0; z<DIMZ; z++)
                if (logicField->get(x,y,z)==NotSet)
                    return false;
    return true;
}



Player ConnectFourGame::isThereAWinner()
{
    Player winner = NotSet;

    // ROUTINE FOR CHECKING STRAIGHT WINS
    // last two indices specify the scanning plane, the first index must be the missing axis
    // the last index specifies the scanning direction
    // ATTENTION: if first winner is found, the rest of the statements will not be evaluated
    if (((winner = checkPlaneInDirection('y', 'x', 'z')) !=NotSet) ||                    //XZ plane in Z direction
        ((winner = checkPlaneInDirection('y', 'z', 'x')) !=NotSet) ||                    //XZ plane in X direction
        ((winner = checkPlaneInDirection('x', 'z', 'y')) !=NotSet) ||                    //YZ plane in Y direction
        ((winner = checkPlaneInDirection('x', 'y', 'z')) !=NotSet) ||                    //YZ plane in Z direction
        ((winner = checkPlaneInDirection('z', 'x', 'y')) !=NotSet) ||                    //XY plane in Y direction
        ((winner = checkPlaneInDirection('z', 'y', 'x')) !=NotSet) ||
        //ROUTINE FOR CHECKING DIAGONAL WINS
        //first and last index specify the plane
        ((winner = checkDiagonalInPlane('x', 'y', 'z')) !=NotSet) ||						//XZ plane
        ((winner = checkDiagonalInPlane('y', 'x', 'z')) !=NotSet) ||						//YZ plane
        ((winner = checkDiagonalInPlane('x', 'z', 'y')) !=NotSet))                    	//XY plane
        ; //nothing todo here. see ATTENTION!
    std::cout << "WINNER:" << (winner==PlayerA ? "PlayerA" : (winner==PlayerB ? "PlayerB" : "No Winner" )) << std::endl;

    return winner;
}


inline void ConnectFourGame::countUp(int &playerAwin, int &playerBwin, int x, int y, int z)
{
    //increment or reset
    Player p = logicField->get(x,y,z);
    if (p==NotSet)
        playerAwin=playerBwin=0;
    else if (p==PlayerA) { playerAwin++; playerBwin=0; }
    else if (p==PlayerB) { playerBwin++; playerAwin=0; }
}

Player ConnectFourGame::checkPlaneInDirection( char iIs, char jIs, char kIs)
{
    int playerAwin = 0, playerBwin = 0;
    int i=0, j=0, k=0;

    //reroute dimensions
    int dim1 = iIs=='x' ? DIMX : (iIs=='y' ? DIMY : (iIs=='z' ? DIMZ : 0));
    int dim2 = jIs=='x' ? DIMX : (jIs=='y' ? DIMY : (jIs=='z' ? DIMZ : 0));
    int dim3 = kIs=='x' ? DIMX : (kIs=='y' ? DIMY : (kIs=='z' ? DIMZ : 0));

    //reroute coordinate variables to point to the right index variable
    int &x = iIs=='x' ? i : (jIs=='x' ? j : (kIs=='x' ? k : i));
    int &y = iIs=='y' ? i : (jIs=='y' ? j : (kIs=='y' ? k : i));
    int &z = iIs=='z' ? i : (jIs=='z' ? j : (kIs=='z' ? k : i));

    for (i=0; i<dim1; i++)
        for (j=0; j<dim2; j++) //x
        {
            playerAwin = playerBwin = 0;
            for (k=0; k<dim3; k++) //Z
            {
                countUp(playerAwin, playerBwin, x,y,z);
                //check if win reached
                if (playerAwin>=WIN)
                    return PlayerA;
                if (playerBwin>=WIN)
                    return PlayerB;
            }
        }
    return NotSet;
}


Player ConnectFourGame::checkDiagonalInPlane(char xIs, char yIs, char zIs)
{
    int playerAwin = 0, playerBwin = 0;		//count-vars for diagonales
    int playerAwinC = 0, playerBwinC = 0;	//count-vars for reverse diagonales

    //for-loop variables
    int x=0, y=0, z=0,  // x, y, z from the origin (0/0/0)
        xC=0, zC=0;		// x, z from the corner (DIMX/0/0), etc. (reverse diagonales)

    //reroute coordinate variables
    int &setX = (xIs=='x' ? x : (xIs=='y' ? y : z));
    int &setY = (yIs=='x' ? x : (yIs=='y' ? y : z));
    int &setZ = (zIs=='x' ? x : (zIs=='y' ? y : z));
    int &setXC = (xIs=='x' ? xC : (xIs=='y' ? y : zC));
    int &setYC = (yIs=='x' ? xC : (yIs=='y' ? y : zC));
    int &setZC = (zIs=='x' ? xC : (zIs=='y' ? y : zC));

    //correct the dimensions
    const int dimX = (xIs=='x' ? DIMX : (xIs=='y' ? DIMY : DIMZ));
    const int dimY = (yIs=='x' ? DIMX : (yIs=='y' ? DIMY : DIMZ));
    const int dimZ = (zIs=='x' ? DIMX : (zIs=='y' ? DIMY : DIMZ));

    for (y=0; y<dimY; y++)
    {
        //int dim= dimX<dimZ ? dimX : dimZ; //the lower dimension defines lenght of longes diagonal
        for (int sIndex=0; sIndex<(dimZ-WIN+1); sIndex++) //sIndex = side index
        {
            x=0;
            z=sIndex;
            xC=0;
            zC=dimZ-sIndex-1;
            playerAwin = playerBwin = playerAwinC = playerBwinC = 0;
            // inc x+1, z+1 till x==DIMX or z==DIMZ (one reaches its index limit)
            while ( x<dimX && x>=0 && z<dimZ && z>=0 &&
                    xC<dimX && xC>=0 && zC<dimZ && zC>=0)
            {
                countUp(playerAwin, playerBwin, setX, setY, setZ);
                countUp(playerAwinC, playerBwinC, setXC, setYC, setZC);
                //check if win reached
                if (playerAwin>=WIN || playerAwinC>=WIN) return PlayerA;
                if (playerBwin>=WIN || playerBwinC>=WIN) return PlayerB;
                x++; z++; xC++; zC--;
            }
        }

        for (int sIndex=1; sIndex<(dimX-WIN+1); sIndex++)
        {
            x=sIndex;
            z=0;
            xC=sIndex;
            zC=dimZ-1;
            playerAwin = playerBwin = playerAwinC = playerBwinC = 0;
            // inc x+1, z+1 till x==DIMX or z==DIMZ (one reaches its index limit)
            while ( x<dimX && x>=0 && z<dimZ && z>=0 &&
                    xC<dimX && xC>=0 && zC<dimZ && zC>=0)
            {
                countUp(playerAwin, playerBwin, setX, setY, setZ);
                countUp(playerAwinC, playerBwinC, setXC, setYC, setZC);
                //check if win reached
                if (playerAwin>=WIN || playerAwinC>=WIN) return PlayerA;
                if (playerBwin>=WIN || playerBwinC>=WIN) return PlayerB;
                x++; z++; xC++; zC--;
            }
        }
    }
    return NotSet;
}
















/*
    //if one of these is 4 --> Win
    int playerAwin = 0;
    int playerBwin = 0;

    for (int i=0; i<logicDIMX; i++)
        for (int j=0; j<logicDIMZ; j++)
        {
            if (logicField->get())
        }
*/

