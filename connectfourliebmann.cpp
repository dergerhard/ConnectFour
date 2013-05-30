#include "connectfourliebmann.h"

ConnectFourLiebmann::ConnectFourLiebmann()
{

    Sett::ings().setString("ui/stylesheet", "QWidget{background-color:black; color:white;}QPushButton {background-color: rgb(72,27,250); border-style: outset;border-width: 2px;border-radius: 10px;border-color: beige;font: bold 12px;min-width: 7em;min-height: 2em;padding: 6px;} QPushButton:hover {background-color: red;} QPushButton:pressed {background-color: rgb(170, 0, 0);} QPushButton[enabled=\"false\"] {background-color: rgb(170, 170, 170);}");
    this->setStyleSheet(Sett::ings().getString("ui/stylesheet"));


    mainLayout = new QHBoxLayout;
    mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);

    statusLayout = new QVBoxLayout;

    statusLayout->addWidget(mainWidget);
    statusBar = new QStatusBar;
    statusLayout->addWidget(statusBar);

    //create navigation
    //ConnectFourNavigationWidget *nav = new ConnectFourNavigationWidget();
    //connect(nav, SIGNAL(quitApplication()), this, SLOT(onQuitClicked()));
    //nav->setMaximumWidth(320);
    //nav->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);
    //mainLayout->addWidget(nav);


    //setStyleSheet("QPushButton{min-width:100px;background-color:black;color:white;}");
    navigation = new QStackedLayout(this);

    //init page
    wInit = new QWidget();
    pInit = new QVBoxLayout();

    btPlayAgainstHuman = new QPushButton("play against human opponent");
    btPlayAgainstAI = new QPushButton("play against artificial intelligence");
    btLetAIPlayAgainstNetworkPlayer = new QPushButton("let the artificial intelligence play");
    btSettings = new QPushButton("settings");
    btHelp = new QPushButton("help");
    btQuit = new QPushButton("quit");

    connect(btQuit, SIGNAL(clicked()), this, SLOT(onBtQuitClicked()));
    connect(btPlayAgainstHuman, SIGNAL(clicked()), this, SLOT(onBtPlayAgainstHumanClicked()));
    connect(btPlayAgainstAI, SIGNAL(clicked()), this, SLOT(onBtPlayAgainstAIClicked()));
    connect(btLetAIPlayAgainstNetworkPlayer, SIGNAL(clicked()), this, SLOT(onBtLetAIPlayAgainstNetworkPlayerClicked()));
    connect(btSettings, SIGNAL(clicked()), this, SLOT(onBtSettingsClicked()));
    connect(btHelp, SIGNAL(clicked()), this, SLOT(onBtHelpClicked()));

    pInit->addWidget(btPlayAgainstHuman);
    pInit->addWidget(btPlayAgainstAI);
    pInit->addWidget(btLetAIPlayAgainstNetworkPlayer);
    pInit->addWidget(btSettings);
    pInit->addWidget(btHelp);
    pInit->addWidget(btQuit);

    wInit->setLayout(pInit);
    navigation->addWidget(wInit);

    //host or join
    wHostOrJoin = new QWidget();
    pHostOrJoin = new QVBoxLayout();

    btHost = new QPushButton("host");
    btJoin = new QPushButton("join");
    btHostOrJoinBack = new QPushButton("go back");

    pHostOrJoin->addWidget(btHost);
    pHostOrJoin->addWidget(btJoin);
    pHostOrJoin->addWidget(btHostOrJoinBack);

    wHostOrJoin->setLayout(pHostOrJoin);
    navigation->addWidget(wHostOrJoin);
    connect(btJoin, SIGNAL(clicked()), this, SLOT(onBtJoinClicked()));

    //pHost - Host game
    lbPlayerName = new QLabel("player name: ");
    lePlayerName = new QLineEdit();
    lbGameName = new QLabel("game name: ");
    leGameName = new QLineEdit();
    lbDimX = new QLabel("width: ");
    leDimX = new QLineEdit();
    lbDimY = new QLabel("height: ");
    leDimY = new QLineEdit();
    lbDimZ = new QLabel("depth: ");
    leDimZ = new QLineEdit();

    pHost = new QFormLayout();
    btHostBack = new QPushButton("go back");
    btDoHost = new QPushButton("host it now");
    pHost->addRow(lbPlayerName,lePlayerName);
    pHost->addRow(lbGameName,leGameName);
    pHost->addRow(lbDimX,leDimX);
    pHost->addRow(lbDimY,leDimY);
    pHost->addRow(lbDimZ,leDimZ);
    pHost->addRow(btHostBack, btDoHost);
    connect(btHost, SIGNAL(clicked()), this, SLOT(onBtHostClicked()));
    connect(btDoHost, SIGNAL(clicked()), this, SLOT(onBtDoHostClicked()));

    wHost = new QWidget();
    wHost->setLayout(pHost);
    navigation->addWidget(wHost);

    //pJoin - Join page
    pJoin = new QVBoxLayout();
    lbGameList = new QLabel("player: game (w/h/d)");
    wFlGameListNav = new QWidget();
    flGameListNav = new QFormLayout();
    wFlGameListNav->setLayout(flGameListNav);

    wJoin = new QWidget();
    btJoinBack = new QPushButton("back");
    btDoJoin = new QPushButton("join game");
    lwGameList = new QListWidget();
    //(Status, Spielername, Spielname, Breite, H ohe, Tiefe, IP-Adresse, Port) x S

    connect(btJoinBack, SIGNAL(clicked()), this, SLOT(onBtBackClicked()));
    connect(btDoJoin, SIGNAL(clicked()), this, SLOT(onBtDoJoinClicked()));
    pJoin->addWidget(lbGameList);
    pJoin->addWidget(lwGameList);

    flGameListNav->addRow(btJoinBack, btDoJoin);
    pJoin->addWidget(wFlGameListNav);
    wJoin->setLayout(pJoin);
    navigation->addWidget(wJoin);

    //set up btBack
    connect(btHostOrJoinBack, SIGNAL(clicked()), this, SLOT(onBtBackClicked()));
    connect(btHostBack, SIGNAL(clicked()), this, SLOT(onBtBackClicked()));

    lbGameProgressStatus = new QLabel("game log");
    lwGameInProgressStatus = new QListWidget();
    btAbortGame = new QPushButton("abort game");
    pGameInProgress = new QVBoxLayout();
    pGameInProgress->addWidget(lbGameProgressStatus);
    pGameInProgress->addWidget(lwGameInProgressStatus);
    pGameInProgress->addWidget(btAbortGame);
    wGameInProgress = new QWidget();
    wGameInProgress->setLayout(pGameInProgress);
    navigation->addWidget(wGameInProgress);
    connect(btAbortGame, SIGNAL(clicked()), this, SLOT(onBtAbortGame()));

    //set layout
    nav = new QWidget(this);
    nav->setLayout(navigation);
    mainLayout->addWidget(nav);

    //create OpenGl widget
    //--------------------------------------------------------------------------------
    //glGame = new ConnectFourGame(this, "test", "testgame", 5,6,7);
    //mainLayout->addWidget(glGame);
    glGame=0;

    lbNoGame = new QLabel("\t connect4liebmann \t");
    lbNoGame->setStyleSheet("QLabel { background-color : black; color : white; font-size:30px; text-align:justified; }");
    mainLayout->addWidget(lbNoGame);

    //connect(nav, SIGNAL(newGame(QString,QString,bool,int,int,int)), this, SLOT(newGame(QString,QString,bool,int,int,int)));

    setLayout(statusLayout);
    setWindowTitle(tr("Connect Four Liebmann"));

    //set up index server connection
    qRegisterMetaType<NetCommand>("NetCommand");
    startIndexClient();

    connect(this, SIGNAL(sEndGame(QString,bool)), this, SLOT(endGame(QString,bool)), Qt::QueuedConnection);

    gameState= NoGame; //not hosted nor joined yet
}


ConnectFourLiebmann::~ConnectFourLiebmann()
{
    
}


void ConnectFourLiebmann::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}

void ConnectFourLiebmann::updateGameList(const NetCommand &games)
{
    QList<QString> parameters = games.getParameters();
    lwGameList->clear();

    int count = parameters.at(0).toInt();
    if (parameters.size()>=1 && count>0)
    {
        indexGameList.clear();
        for (int i=0; i<count; i++)
        {
            //stores all games from indexserver
            indexGameList.append(Game(parameters.at(i*8+1).toUpper()=="OPEN" ? true :false,
                                     parameters.at(i*8+2),
                                     parameters.at(i*8+3),
                                     parameters.at(i*8+4).toInt(),
                                     parameters.at(i*8+5).toInt(),
                                     parameters.at(i*8+6).toInt(),
                                     parameters.at(i*8+7),
                                     parameters.at(i*8+8).toInt()
                                     ));

            //append game to UI list
            Game game = indexGameList.at(indexGameList.size()-1);
            QListWidgetItem *it1;
            if (game.open)
                it1 = new QListWidgetItem(QIcon(":/images/open.xpm"), game.toGuiString());
            else it1 = new QListWidgetItem(QIcon(":/images/sealed.xpm"), game.toGuiString());
            lwGameList->insertItem(0, it1);
        }
    }


}



void ConnectFourLiebmann::localPlayerMoved(const QVector3D &move)
{
    addGameInProgressStatus(QString("you moved to ")
                            .append(QString::number((int)move.x())).append("/")
                            .append(QString::number((int)move.y())).append("/")
                            .append(QString::number((int)move.z())));


    NetCommand cmd(MOVE, QString::number(glGame->getIDFromMove(move)));

    if (gameState==JoinedGameInProgress)
        joinCom->sendCommand(cmd);
    if (gameState==HostedGameInProgress)
        hostCom->sendCommand(cmd);
}

void ConnectFourLiebmann::netPlayerMoved(const QVector3D &move)
{
    addGameInProgressStatus(QString("you moved to ")
                            .append(QString::number((int)move.x())).append("/")
                            .append(QString::number((int)move.y())).append("/")
                            .append(QString::number((int)move.z())));



    NetCommand cmd(SYNCHRONIZE_GAME_BOARD, QString::number(glGame->DIMX*glGame->DIMY*glGame->DIMZ));

    for (int x=0; x<glGame->DIMX; x++)
        for (int y=0; y<glGame->DIMY; y++)
            for (int z=0; z<glGame->DIMZ; z++)
            {
                int moveID = glGame->getIDFromMove(QVector3D(x,y,z));
                Player state = glGame->getFieldState(QVector3D(x,y,z));
                int netState=0;
                if (gameState==JoinedGameInProgress)
                    netState = state==PlayerA ? 2 : 1;
                if (gameState==HostedGameInProgress)
                    netState = state==PlayerA ? 1 : 2;
                cmd.addParameter(QString::number(moveID));
                cmd.addParameter(QString::number(netState));
            }

    if (gameState==JoinedGameInProgress)
        joinCom->sendCommand(cmd);
    if (gameState==HostedGameInProgress)
        hostCom->sendCommand(cmd);
}



void ConnectFourLiebmann::newGame(const QString &playerName, const QString &gameName, bool state, int x, int y, int z, Player turn)
{
    mainLayout->removeWidget(lbNoGame);
    if (glGame!=0)
        delete glGame;
    glGame = new ConnectFourGame(this, playerName, gameName, x,y,z, turn);
    glGame->setState(state);
    connect(glGame, SIGNAL(localPlayerMoved(QVector3D)), this, SLOT(localPlayerMoved(QVector3D)));

    mainLayout->addWidget(glGame);
}


void ConnectFourLiebmann::endGame(QString msg, bool yesno)
{
    int res;
    if (yesno)
        res = QMessageBox::information(this, "info", msg, QMessageBox::Yes, QMessageBox::No);
    else
        QMessageBox::information(this, "info", msg);

    if ((yesno && res==QMessageBox::Yes) || !yesno)
    {
        mainLayout->removeWidget(glGame);
        delete glGame;
        glGame=0;
        mainLayout->addWidget(lbNoGame);

        wLastPage.clear();
        navigation->setCurrentWidget(wInit);

        if (gameState==JoinedGameInProgress)
            endJoinClient();
        else endHostServer();

        gameState = NoGame;
    }

}

void ConnectFourLiebmann::onQuitClicked()
{
    if (QMessageBox::question(this, "quit", "are you sure, you want to quit?",
                              QMessageBox::No, QMessageBox::Yes)==QMessageBox::Yes)
        this->close();
}







void ConnectFourLiebmann::onBtAbortGame()
{
    emit endGame("do you really want to abort the game?", true);
}


void ConnectFourLiebmann::onBtQuitClicked()
{
    //emit quitApplication();
}

void ConnectFourLiebmann::onBtBackClicked()
{
    if (wLastPage.size()>0)
        navigation->setCurrentWidget(wLastPage.pop());
}


void ConnectFourLiebmann::onBtPlayAgainstHumanClicked()
{
    wLastPage.push(wInit);
    navigation->setCurrentWidget(wHostOrJoin);
}

void ConnectFourLiebmann::onBtPlayAgainstAIClicked()
{
    wLastPage.push(wInit);
    navigation->setCurrentWidget(wHostOrJoin);
}

void ConnectFourLiebmann::onBtLetAIPlayAgainstNetworkPlayerClicked()
{
    wLastPage.push(wInit);
    navigation->setCurrentWidget(wHostOrJoin);
}

void ConnectFourLiebmann::onBtSettingsClicked()
{
    ConnectFourSettings *settingsWindows = new ConnectFourSettings(this);

    settingsWindows->show();
}

void ConnectFourLiebmann::onBtHelpClicked()
{

    QMessageBox::information(this, "help",
                             "the game is played as followed:\n - move left/right/up/down to move your selector\n - hit space to put down your cube\n - to see hidden cubes, hit shift-key - for deactivating hidden-mode, hit shift-key again", QMessageBox::Ok);
}

void ConnectFourLiebmann::onBtHostClicked()
{
    lePlayerName->setText(Sett::ings().getString("net/playername"));
    leGameName->setText(Sett::ings().getString("net/gamename"));
    wLastPage.push(wHostOrJoin);
    navigation->setCurrentWidget(wHost);
}

void ConnectFourLiebmann::onBtJoinClicked()
{
    if (indexCom==0 || !indexCom->isConnected())
    {
        QMessageBox::warning(this, "fatal error", "the index server is down. this application will be closed now!", QMessageBox::Ok);
        this->close();
    }

    netRequestGameList();

    wLastPage.push(wHostOrJoin);
    navigation->setCurrentWidget(wJoin);
}


void ConnectFourLiebmann::onBtDoHostClicked()
{
    QString gameName = leGameName->text();
    QString playerName = lePlayerName->text();
    bool *xIsInt, *yIsInt, *zIsInt;
    xIsInt = new bool; *xIsInt = false;
    yIsInt = new bool; *yIsInt = false;
    zIsInt = new bool; *zIsInt = false;

    int x = leDimX->text().toInt(xIsInt);
    int y = leDimY->text().toInt(yIsInt);
    int z = leDimZ->text().toInt(zIsInt);
    bool dimNotEmpty = ((leDimX->text()!="") && (leDimY->text()!="") && (leDimZ->text()!="")) ;

    if (gameName!="" && playerName!="" && xIsInt && yIsInt && zIsInt &&
            x>0 && y>0 && z>0 && dimNotEmpty)
    {
        if ((x*y*z)%2!=0)
            QMessageBox::warning(this, "input error", "you can only choose a game with an even number of fields - otherwise one player would haven an advantage!");
        else
        {
            Sett::ings().setString("net/playername", playerName);
            Sett::ings().setString("net/gamename", gameName);

            indexCom->sendCommand(NetCommand(REGISTER_GAME, playerName, gameName, QString::number(x),QString::number(y),QString::number(z), "1.1.1.1", "80"));
            emit newGame(playerName, gameName, true, x,y,z, PlayerA);
            wLastPage.clear();
            navigation->setCurrentWidget(wGameInProgress);

            this->clearGameInProgressStatus();

            addGameInProgressStatus("Game hosted on index server.");
            addGameInProgressStatus("Waiting for player...");

            startHostServer();
        }
    }
    else QMessageBox::warning(this, "input error", "your input is invalid. \"game name\" and \"player name\" must be filled out, \"width\", \"height\" and \"depth\" must be numbers and greater than 0!", QMessageBox::Ok);


}


void ConnectFourLiebmann::onBtDoJoinClicked()
{
    if (lwGameList->selectedItems().size()==1)
    {
        Game g;
        bool isGame = false;
        QString game = lwGameList->selectedItems().at(0)->text();
        for (int i=0; i<indexGameList.size(); i++)
        {
            Game tmpG = indexGameList.at(i);
            QString tmpS = tmpG.toGuiString();
            if (game==tmpS)
            {
                g = indexGameList.at(i);
                isGame = true;
            }
        }

        if (!g.open)
            QMessageBox::information(this, "error", "this game is not open. you can't join it.");
        else
        {
            gameToBeJoined = g;
            emit newGame(gameToBeJoined.playerName, gameToBeJoined.gameName, true, gameToBeJoined.x,gameToBeJoined.y,gameToBeJoined.z, PlayerB);
            //WARNING: emit newGame() does not work after startJoinClient()... threading problem

            wLastPage.clear();
            navigation->setCurrentWidget(wGameInProgress);
            clearGameInProgressStatus();
            //addGameInProgressStatus("opponents turn...");

            startJoinClient(g.ip, g.port);
            joinCom->sendCommand(NetCommand(JOIN_GAME, Sett::ings().getString("net/playername"), g.gameName, "V1"));
            gameState = GameJoinInProgress;

            if (QMessageBox::Yes == QMessageBox::question(this, "who will start?", "do you want to start? (or let your opponent start)", QMessageBox::Yes, QMessageBox::No))
                glGame->setTurn(PlayerA);
            else
                glGame->setTurn(PlayerB);

        }

    }
    else
        QMessageBox::information(this, "error", "you have to select a game", QMessageBox::Ok);
}




void ConnectFourLiebmann::addGameInProgressStatus(QString msg)
{
    lwGameInProgressStatus->addItem(new QListWidgetItem(msg));
}

void ConnectFourLiebmann::clearGameInProgressStatus()
{
    lwGameInProgressStatus->clear();
}

void ConnectFourLiebmann::netRequestGameList()
{
    NetCommand cmd(REQUEST_GAME_LIST);
    indexCom->sendCommand(cmd);
}


void ConnectFourLiebmann::indexServerCommandReceived(const NetCommand &cmd)
{
    const QString msg = cmd.getMessage();
    //no game established yet-----------------------------------------
    if (gameState==NoGame)
    {
        if (msg=="register_success" || msg=="register_game_success")
        {
            //statusBar->showMessage("Game registered successfully!", 5000);
            netRequestGameList();
        }
        else if (msg=="register_failed")
        {
            QMessageBox::critical(this,
                                 "Register game failed",
                                 "Registring the game failed. Message from server:\n" + cmd.getParameter(0) ,
                                 QMessageBox::Ok);
            statusBar->showMessage("Registration of the game failed", 5000);
            netRequestGameList();
        }
        else if (msg=="answer_game_list")
        {
            //statusBar->showMessage("Game list updated successfully", 5000);
            updateGameList(cmd);
        }
        else if (msg=="unregister_game_failed")
        {
            QMessageBox::critical(this,
                                 "Unregister game failed",
                                 "Unregistring the game failed. Message from server:\n" + cmd.getParameter(0) ,
                                 QMessageBox::Ok);
            statusBar->showMessage("Unregistration of the game failed", 5000);
            netRequestGameList();
        }
        else if (msg=="unregister_game_success")
        {
            statusBar->showMessage("Game unregistred successfully");
            netRequestGameList();
        }
    }
    //me == opponent---------------------------------------------------
    else if (gameState==GameJoined)
    {
        if (msg=="join_game_success")
        {

        }
        else if (msg=="join_game_failed")
        {

        }
        else if (msg=="start_game")
        {

        }
        else if (msg=="moved_failed")
        {

        }
        else if (msg=="end_game")
        {

        }

    }
    //me == initiator---------------------------------------------------
    else if (gameState==GameHosted)
    {
        //
        //join_game
        //Eigener Spielername, Spielname, Protokollversion
        if (msg=="join_game")
        {
            QString question = "the player \"";
            question.append(cmd.getParameter(1)).append("\" wants to join your game \"");
            question.append(cmd.getParameter(2)).append("\". do you want to play?");
            QMessageBox::question(this, "join request", question, QMessageBox::Yes, QMessageBox::No);
        }
    }


    //sync and abort happens as host and as opponent--------------------
    if (gameState==GameHosted || gameState==GameJoined)
    {
        if (msg=="synchronize_game_board") //V1 - answer to move
        {

            statusBar->showMessage("Game board synchronized...");
        }
        else if (msg=="updated_game_board") //V2 - answer to move
        {

            statusBar->showMessage("Game board updated...");
        }
        else if (msg=="abort_game")
        {

        }
    }

    cout << "received: " << cmd.toString().toStdString() << endl;
}

void ConnectFourLiebmann::startHostServer()
{
    hostServerThread = new QThread;
    hostCom = new NetServerCom();

    connect(hostServerThread, SIGNAL(started()), hostCom, SLOT(startListening()));
    connect(hostServerThread, SIGNAL(finished()), hostServerThread, SLOT(deleteLater()));
    connect(hostCom, SIGNAL(finished()), hostServerThread, SLOT(quit()));
    connect(hostCom, SIGNAL(finished()), hostCom, SLOT(deleteLater()));
    connect(hostCom, SIGNAL(commandReceived(NetCommand)), this, SLOT(hostedGameCommandReceived(NetCommand)), Qt::DirectConnection);
    connect(hostCom, SIGNAL(lostConnection(bool)), this, SLOT(lostHostServerConnection(bool)), Qt::QueuedConnection);

    hostCom->moveToThread(hostServerThread);
    hostServerThread->start();
}

void ConnectFourLiebmann::endHostServer()
{
    hostCom->endCommunication();
    hostServerThread->quit();
    delete hostCom;
    delete hostServerThread;
}

void ConnectFourLiebmann::lostHostServerConnection(bool comActive)
{
    if (comActive) //if comActive --> error, otherwise we wanted to close
    {
        QMessageBox::warning(this, "error", "lost connection to client. the game will be closed.");
        endHostServer();
        close();
    }

}


void ConnectFourLiebmann::hostedGameCommandReceived(const NetCommand &cmd)
{
    cout << "COMMAND RECEIVED: " << cmd.toString().toStdString() << endl;
}


void ConnectFourLiebmann::startIndexClient()
{
    indexClientThread = new QThread;
    indexCom = new NetClientCom(Sett::ings().getString("net/indexip"), Sett::ings().getInt("net/indexport"));

    connect(indexClientThread, SIGNAL(started()), indexCom, SLOT(startCommunication()));
    connect(indexClientThread, SIGNAL(finished()), indexClientThread, SLOT(deleteLater()));
    connect(indexCom, SIGNAL(finished()), indexClientThread, SLOT(quit()));
    connect(indexCom, SIGNAL(finished()), indexCom, SLOT(deleteLater()));
    connect(indexCom, SIGNAL(commandReceived(NetCommand)), this, SLOT(indexServerCommandReceived(NetCommand)), Qt::DirectConnection);
    connect(indexCom, SIGNAL(lostConnection(bool)), this, SLOT(lostIndexClientConnection(bool)), Qt::QueuedConnection);

    indexCom->moveToThread(indexClientThread);
    indexClientThread->start();
}

void ConnectFourLiebmann::endIndexClient()
{
    hostCom->endCommunication();
    hostServerThread->quit();
    delete hostCom;
    delete hostServerThread;
}

void ConnectFourLiebmann::lostIndexClientConnection(bool comActive)
{
    if (comActive)
    {
        QMessageBox::warning(this, "error", "lost connection to index server. the game will be closed.");
        endIndexClient();
        close();
    }

}





void ConnectFourLiebmann::startJoinClient(QString ip, int port)
{
    joinClientThread = new QThread;
    joinCom = new NetClientCom(ip, port);

    /*
    connect(indexClientThread, SIGNAL(started()), indexCom, SLOT(startCommunication()));
    connect(indexClientThread, SIGNAL(finished()), indexClientThread, SLOT(deleteLater()));
    connect(indexCom, SIGNAL(finished()), indexClientThread, SLOT(quit()));
    connect(indexCom, SIGNAL(finished()), indexCom, SLOT(deleteLater()));
    connect(indexCom, SIGNAL(commandReceived(NetCommand)), this, SLOT(indexServerCommandReceived(NetCommand)), Qt::DirectConnection);
    connect(indexCom, SIGNAL(lostConnection()), this, SLOT(lostIndexClientConnection()));
    */

    connect(joinClientThread, SIGNAL(started()), joinCom, SLOT(startCommunication()));
    connect(joinCom, SIGNAL(finished()), joinClientThread, SLOT(quit()));
    connect(joinCom, SIGNAL(commandReceived(NetCommand)), this, SLOT(joinedGameCommandReceived(NetCommand)), Qt::DirectConnection);
    connect(joinCom, SIGNAL(lostConnection(bool)), this, SLOT(lostJoinClientConnection(bool)), Qt::QueuedConnection);

    joinCom->moveToThread(joinClientThread);
    joinClientThread->start();
}

void ConnectFourLiebmann::endJoinClient()
{
    joinCom->endCommunication();

    if(!joinClientThread->wait(100))
    {
      //thread deadlock
      joinClientThread->terminate(); //terminate it
      joinClientThread->wait();
    }

    delete joinCom;
    delete joinClientThread;
}


void ConnectFourLiebmann::lostJoinClientConnection(bool comActive)
{
    if (comActive)
    {
        QMessageBox::warning(this, "error", "lost connection to hosting server. the game will be closed.");
        endJoinClient();
        close();
    }

}

//commands from hosting server
void ConnectFourLiebmann::joinedGameCommandReceived(const NetCommand &cmd)
{
    QString msg = cmd.getMessage();

    if (gameState==GameJoinInProgress)
    {
        if (msg=="join_game_success")
        {
            if (cmd.getParameter(0).toUpper()=="V1")
                protocol= V1;
            else protocol = V2;

            gameState =JoinedGameInProgress;
            addGameInProgressStatus(QString("joined the game \"")
                                    .append(gameToBeJoined.gameName).append("\" with \"").append(gameToBeJoined.playerName).append("\" successfully!"));
        }
        else if (msg=="join_game_failed")
        {
            QMessageBox::information(this, "error", QString("could not join the game. the server says: ").append(cmd.getParameter(2)));
        }
    }
    else if (gameState==JoinedGameInProgress)
    {
        /*possible commands:
         *  synchronize_game_board
         *  updated_game_board
         *  moved_failed
         *  end_game
         *  abort_game
         *  move
         *
         */
        if (msg=="move")
        {
            QVector3D move = glGame->getMoveFromID(cmd.getParameter(0).toInt());

            if ((move.x()==-1 && move.y()==-1 && move.z()==-1) || (move.x()==-2 && move.y()==-2 && move.z()==-2))
            {
                QString errorMsg;

                if (move.x()==-1)
                    errorMsg = "your move is outside the grid! game aborted";
                else
                    errorMsg = "this field is already set! game aborted";

                joinCom->sendCommand(NetCommand(MOVED_FAILED, errorMsg));
                emit sEndGame("your opponent sent an invalid move. the game is aborted", false);
            }


        }
        else if (msg=="synchronize_game_board")
        {
            QList<int> states;
            int i=1;
            for (int x=0; x<glGame->DIMX; x++)
                for (int y=0; y<glGame->DIMY; y++)
                    for (int z=0; z<glGame->DIMZ; z++)
                    {
                        i++;
                        int state = cmd.getParameter(i++).toInt();
                        Player fieldState;

                        //convert net notation to local notation
                        if (state==0)
                            fieldState=NotSet;
                        else
                        {   //joined game: host Player=1
                            //             my   Player=2

                            //hosted game: my     Player=1
                            //             joined Player=2
                            fieldState = state==2 ? PlayerA : PlayerB;
                        }
                        states.append(fieldState);
                        cout << "setting fieldState (" << fieldState << ") to " << x << y << z << endl;
                    }

            if (!glGame->syncGameBoard(states))
            {
                emit sEndGame("board synchronisation error. the game will be closed", false);
                hostCom->sendCommand(NetCommand(MOVED_FAILED, "synchronization didn't work"));
            }
        }
        else if (msg=="update_game_board")
        {
            int moveid = cmd.getParameter(0).toInt();
            int state = cmd.getParameter(1).toInt();


        }
        else if (msg=="moved_failed")
        {
            emit sEndGame(QString("one of your moves failed - the game will be closed. the host says that is wrong:").append(cmd.getParameter(0)), false);
        }
        else if (msg=="end_game")
        {
            QString msg;
            if (cmd.getParameter(0).toInt()==0)
                msg = "game over! you reached a draw! no one won, or both ;)";
            else if (cmd.getParameter(0).toInt()==1)
                msg = "game over! you lost!";
            if (cmd.getParameter(0).toInt()==2)
                msg = "congratulations! you won!";
            emit sEndGame(msg, false);
        }
        else if (msg=="abort_game")
        {
            emit sEndGame(QString("your opponent quit the game. his reason:").append(cmd.getParameter(0)), false);
        }
    }


}


