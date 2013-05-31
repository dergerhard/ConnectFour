#ifndef CONNECTFOURLIEBMANN_H
#define CONNECTFOURLIEBMANN_H

#include <QMainWindow>
#include <QtGui>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListView>
#include <QStackedLayout>
#include <QStack>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QSettings>
#include <QTableWidget>
#include <QListWidget>
#include <iostream>

#include "settings.h"
#include "netcommand.h"
#include "netclientcom.h"
#include "netservercom.h"
#include "settings.h"
#include "connectfourgame.h"
#include "connectfoursettings.h"

enum GameState { NoGame, GameHosted, GameJoined , GameJoinInProgress, JoinedGameInProgress, HostedGameInProgress};
enum Protocol { V1, V2 };

struct Game
{
    bool open=false;
    QString playerName;
    QString gameName;
    int x=0, y=0, z=0;
    QString ip;
    int port;
    QString guid;

    Game()
    {
    }

    Game(bool open, QString playerName, QString gameName, int x, int y, int z, QString ip, int port, QString guid="")
    {
        this->open=open;
        this->playerName=playerName;
        this->gameName=gameName;
        this->x=x;
        this->y=y;
        this->z=z;
        this->ip=ip;
        this->port=port;
        this->guid=guid;
    }

    bool operator==(const Game &b)
    {
        if (this->gameName==b.gameName) //gameName is enought distinction (on the server every game has to have a different name)
            return true;
        return false;
    }

    QString toGuiString()
    {
        QString game;
        game.append(playerName)
                .append(": ")
                .append(gameName)
                .append(" (")
                .append(QString::number(x))
                .append("/")
                .append(QString::number(y))
                .append("/")
                .append(QString::number(z))
                .append(")");
        return game;
    }

};

class ConnectFourLiebmann : public QWidget
{
    Q_OBJECT
private:

    //Stacked Layout
    QStackedLayout *navigation;
    QWidget *nav; //container for stacked layout

    //Stacked pages
    QWidget *wGameInProgress;
    QVBoxLayout *pGameInProgress;


    //pGameInPrograss
    QLabel *lbGameProgressStatus;
    QListWidget *lwGameInProgressStatus;
    QPushButton *btAbortGame;


    //NETWORK COMMUNICATION
    NetClientCom *joinCom;    //communications thread for the actual game
    NetServerCom *hostCom;

    QThread *hostServerThread;
    QThread *joinClientThread;


    Game gameToBeJoined;

    void addGameInProgressStatus(QString msg);
    void clearGameInProgressStatus();

    QHBoxLayout *mainLayout;
    QWidget *mainWidget;
    ConnectFourGame *glGame;

    const Game game;        //info about game
    GameState gameState;    //state of the game
    Protocol protocol;      //protocol version


public:
    ConnectFourLiebmann(Game gameName, GameState state);
    ~ConnectFourLiebmann();

protected:
    void keyPressEvent(QKeyEvent *event);

signals:
    void sEndGame(QString msg, bool yesno);
    void joinRequestReceived(const NetCommand &cmd);

public slots:
    //if glGame makes a move (local computer or human player)
    void localPlayerMoved(const QVector3D &move);
    void netPlayerMoved(const QVector3D &move);

    //commands from hosting server
    void joinedGameCommandReceived(const NetCommand &cmd);
    void startJoinClient();
    void endJoinClient();
    void lostJoinClientConnection(bool comActive);
    void joinGame();

    //commands from client
    void hostedGameCommandReceived(const NetCommand &cmd);
    void startHostServer();
    void endHostServer();
    void lostHostServerConnection(bool comActive);

    void newGame(Game g, bool state);
    void endGame(QString msg, bool yesno);
    void onQuitClicked();

    //wInit
    void onBtQuitClicked();

    //wGameInProgress
    void onBtAbortGame();
};



#endif
