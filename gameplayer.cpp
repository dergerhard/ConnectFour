#include "gameplayer.h"

GamePlayer::GamePlayer(QObject *parent) :
    QObject(parent)
{
}



ComputerGamePlayer::ComputerGamePlayer(ConnectFourGame *game, ArrayDList<Player> *field)
{
    this->game = game;
    this->field=field;
}



QVector<QVector3D> ComputerGamePlayer::getLegalMoves()
{
    QVector<QVector3D> moves;

    for (int x=0; x<game->DIMX; x++)
        for (int z=0; z<game->DIMZ; z++)
        {
            //find the vertical index (in wich height will the cube be placed)
            int y=-1;
            for (int i=game->DIMY-1;i>=0; --i)
            {
                if  (field->get(x,i,z)==NotSet)
                    y=i;
            }
            if (y!=-1)
                moves.append(QVector3D(x,y,z));
        }

    return moves;
}


int ComputerGamePlayer::getBoardScore()
{   //Player is a enum, PlayerA=1, PlayerB=-1, NotSet=0
    return game->isThereAWinner();

}


double diffclock(clock_t clock1,clock_t clock2)
{
    double diffticks=clock1-clock2;
    double diffms=(diffticks*1000)/CLOCKS_PER_SEC;
    return diffms;
}


int ComputerGamePlayer::analysis(int depth)
{
    //return   checkForWin()*depth;
}

bool ComputerGamePlayer::isBoardFull()
{
    for (int x=0; x<game->DIMX; x++)
        for (int y=0; y<game->DIMY; y++)
            for (int z=0; z<game->DIMZ; z++)
                if (field->get(x,y,z)==NotSet)
                    return false;
    return true;
}


int evaluationCount=0;

int ComputerGamePlayer::negamax(Player p, int depth, int maxA, int minB, QVector3D &winmove, int level, double startTime)
{
    /*
    evaluationCount++;
    if (diffclock(clock(), startTime)>5000)
    {
        cout << "Level reached: " << level << endl;
        return p*analysis(depth);
    }

    if (depth==0 || isBoardFull() || checkForWin() )
        return p*analysis(depth);

    int maxScore = maxA;
    bool foundPV = false;

    //get legal moves
    Vector3D moves[game->DIMX*game->DIMY*game->DIMZ];
    int mI=0;
    int middle = floor(game->DIMX/2);

    //extended version - middle to boarder
    int xR=middle;
    int xL=middle-1;
    bool turnR = true;
    while (xL>=0 || xR<game->DIMX)
    {
        int x;
        if (turnR)
            x=xR++;
        else x=xL--;
        turnR = ! turnR;
        //cout << "x: " << x << endl;
        int y=-1;
        for (int i=0;i<game->DIMY; i++)
            if (field[x][i][0]==N)
                y=i;
        if (y!=-1)
        {
            moves[mI]=Vector3D(x,y,0);
            mI++;
        }
    }

    char a='a';
    for (int i=0; i<mI; i++)
    {
        Vector3D m= moves[i];
        //make move
        field[m.x][m.y][m.z] = p;
        //get score by evaluating child nodes
        //int score = -negamax4((p==A?B:A), depth-1, -minB, -maxScore, winmove, level+1, startTime);
        int score = 0;
        if (foundPV)
        {
            score = -negamax4((p==A?B:A), depth-1, -maxA-1, -maxA, winmove, level+1, startTime);
            if (score > maxA && score < minB)
                score = -negamax4((p==A?B:A), depth-1, -minB, -score, winmove, level+1, startTime);
        } else
            score = -negamax4((p==A?B:A), depth-1, -minB, -maxA, winmove, level+1, startTime);

        //undo move
        field[m.x][m.y][m.z] = N;

        if (score > maxScore)
        {
             if (level == 0)
            {
                winmove.x= m.x;
                winmove.y= m.y;
                winmove.z= m.z;
            }
            if (score >= minB)
                return score;
            maxScore = score;
            if (score > maxA)
            {
                maxA = score;
                foundPV = true;
            }
        }



        a++;
    }
    return maxScore;*/

}




/**
 * @brief Implementation of Negamax algorithm
 *
 */
QVector3D ComputerGamePlayer::getNextMove()
{

}
