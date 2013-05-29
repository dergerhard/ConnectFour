#ifndef CONNECTFOURMANAGEMENT_H
#define CONNECTFOURMANAGEMENT_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>

#include "settings.h"
#include "netclientcom.h"
#include "netcommand.h"
#include "connectfoursettings.h"
#include "connectfourliebmann.h"

class ConnectFourManagement : public QWidget
{
    Q_OBJECT
    QVBoxLayout *vbMain;
    QHBoxLayout *hbMain;
    QVBoxLayout *vbMainLeft;
    QVBoxLayout *vbMainRight;

    QWidget *wMain;
    QWidget *wMainLeft;
    QWidget *wMainRight;


    QLabel *lbHeading;

    //hosting games
    QWidget *wHost;
    QFormLayout *flHost;
    QLabel  *lbPlayerName,
            *lbGameName,
            *lbDimX,
            *lbDimY,
            *lbDimZ;

    QLineEdit   *lePlayerName,
                *leGameName,
                *leDimX,
                *leDimY,
                *leDimZ;

    QPushButton *btHost;


    //Settings/Help/Quit
    QVBoxLayout *vbSetHelpQuit;
    QWidget     *wSetHepQuit;
    QPushButton *btSettings,
                *btHelp,
                *btQuit;

    //####RIGHT COLUMN#############################

    //games list
    QLabel *lbIndexGames;
    QListWidget *lwIndexGamesList;
    QPushButton *btRefreshIndexGameList;
    QPushButton *btDeleteIndexGame;
    QPushButton *btPlayIndexGame;
    QFormLayout *flIndexGames;
    QWidget *wIndexGames;

    //log
    QLabel *lbLog;
    QListWidget *lwLog;
    QVBoxLayout *vbLog;
    QWidget*wLog;

    //####NETWORK COMMUNICATION#######################
    NetClientCom *indexCom;   //communications thread for index server
    QThread *indexClientThread;

    //####MISC ######################################
    ConnectFourSettings *settingsWindow=0;

    QList<Game> indexGameList;
    QList<Game> myHostedGames;
    Game *lastToBeHostedOnIndexServer=0;
    int nextHostPort;


public:
    explicit ConnectFourManagement(QWidget *parent = 0);
    
    void createLeftColumn();
    void createRightColumn();

    void netDeleteGame(const Game &g);

private:
    void updateGameList(const NetCommand &games);
    void logAddEntry(const QString &msg);
    void logClear();

signals:
    
public slots:
    void onBtHostClicked();

    void onBtSettingsClicked();
    void onBtHelpClicked();
    void onBtQuitClicked();

    void onBtDeleteGame();
    void onBtJoinGame();

    //commands from index server
    void indexClientCommandReceived(const NetCommand &cmd);
    void indexClientStart();
    void indexClientEnd();
    void indexClientLostConnection(bool comActive);

    void netRequestGameList();

};

#endif // CONNECTFOURMANAGEMENT_H
