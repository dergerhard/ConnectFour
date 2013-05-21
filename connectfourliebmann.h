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

    Game()
    {
    }

    Game(bool open, QString playerName, QString gameName, int x, int y, int z, QString ip, int port)
    {
        this->open=open;
        this->playerName=playerName;
        this->gameName=gameName;
        this->x=x;
        this->y=y;
        this->z=z;
        this->ip=ip;
        this->port=port;
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

    QList<Game> gameList;

    //Stacked Layout
    QStackedLayout *navigation;
    QWidget *nav; //container for stacked layout

    //back button
    QPushButton *btBack;
    QStack<QWidget*> wLastPage;

    //Stacked pages
    QWidget *wInit,
            *wHostOrJoin,
            *wHost,
            *wJoin,
            *wGameInProgress;

    QVBoxLayout *pInit,
                *pHostOrJoin;
    QFormLayout *pHost;
    QVBoxLayout *pJoin,
                *pGameInProgress;

    //pInit
    QPushButton *btPlayAgainstHuman,
                *btPlayAgainstAI,
                *btLetAIPlayAgainstNetworkPlayer,
                *btSettings,
                *btHelp,
                *btQuit;

    //pHostOrJoin
    QPushButton *btHost,
                *btJoin,
                *btHostOrJoinBack;

    //pHost
    QLabel *lbPlayerName;
    QLineEdit *lePlayerName;
    QLabel *lbGameName;
    QLineEdit *leGameName;
    QLabel *lbDimX;
    QLineEdit *leDimX;
    QLabel *lbDimY;
    QLineEdit *leDimY;
    QLabel *lbDimZ;
    QLineEdit *leDimZ;
    QPushButton *btDoHost;
    QPushButton *btHostBack;

    //pJoin
    QPushButton *btJoinBack;
    QPushButton *btDoJoin;
    QListWidget *lwGameList;
    QLabel *lbGameList;
    QWidget *wFlGameListNav;
    QFormLayout *flGameListNav;

    //pGameInPrograss
    QLabel *lbGameProgressStatus;
    QListWidget *lwGameInProgressStatus;
    QPushButton *btAbortGame;

    QLabel *lbNoGame;

    //NETWORK COMMUNICATION
    NetClientCom *indexCom;   //communications thread for index server
    NetClientCom *joinCom;    //communications thread for the actual game
    NetServerCom *hostCom;

    QThread *hostServerThread;
    QThread *indexClientThread;
    QThread *joinClientThread;

    GameState gameState;
    Protocol protocol;
    Game gameToBeJoined;

    void updateGameList(const NetCommand &games);
    void netRequestGameList();
    void addGameInProgressStatus(QString msg);
    void clearGameInProgressStatus();

public:
    ConnectFourLiebmann();
    ~ConnectFourLiebmann();


protected:
    void keyPressEvent(QKeyEvent *event);

private:
    QVBoxLayout *statusLayout;
    QHBoxLayout *mainLayout;
    QWidget *mainWidget;
    ConnectFourGame *glGame;
    QStatusBar *statusBar;

signals:
    void sEndGame(QString msg, bool yesno);

public slots:
    //if glGame makes a move (local computer or human player)
    void localPlayerMoved(const QVector3D &move);
    void netPlayerMoved(const QVector3D &move);

    //commands from index server
    void indexServerCommandReceived(const NetCommand &cmd);
    void startIndexClient();
    void endIndexClient();
    void lostIndexClientConnection(bool comActive);

    //commands from hosting server
    void joinedGameCommandReceived(const NetCommand &cmd);
    void startJoinClient(QString ip, int port);
    void endJoinClient();
    void lostJoinClientConnection(bool comActive);

    //commands from client
    void hostedGameCommandReceived(const NetCommand &cmd);
    void startHostServer();
    void endHostServer();
    void lostHostServerConnection(bool comActive);

    void newGame(const QString &playerName, const QString &gameName, bool state ,int x, int y, int z, Player turn);
    void endGame(QString msg, bool yesno);
    void onQuitClicked();

    //wInit
    void onBtQuitClicked();
    void onBtBackClicked();
    void onBtPlayAgainstHumanClicked();
    void onBtPlayAgainstAIClicked();
    void onBtLetAIPlayAgainstNetworkPlayerClicked();
    void onBtSettingsClicked();
    void onBtHelpClicked();

    //wHostOrJoin
    void onBtHostClicked();
    void onBtJoinClicked();

    //wHost
    void onBtDoHostClicked();

    //wJoin
    void onBtDoJoinClicked();

    //wGameInProgress
    void onBtAbortGame();
};



#endif
