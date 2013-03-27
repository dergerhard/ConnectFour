#ifndef GAMEPLAYER_H
#define GAMEPLAYER_H

#include <QObject>
#include <QVector3D>
#include <time.h>
#include "collections.h"
#include "connectfourgame.h"
#include "graphicobjects.h"

enum Player;

class GamePlayer : public QObject
{
    Q_OBJECT
public:
    explicit GamePlayer(QObject *parent = 0);
    virtual QVector3D getNextMove()=0;

private:

signals:

public slots:

};


class ComputerGamePlayer : public GamePlayer
{
    Q_OBJECT
public:
    ComputerGamePlayer(ConnectFourGame *game, ArrayDList<Player> *field);
    QVector3D getNextMove(); //calls negamax

private:
    int negamax(Player p, int depth, int maxA, int minB, QVector3D &winmove, int level, double startTime);
    ConnectFourGame *game;
    ArrayDList<Player> *field;
    bool isBoardFull();
    int analysis(int depth);

    QVector<QVector3D> getLegalMoves();
    int getBoardScore();

};

#endif // GAMEPLAYER_H
