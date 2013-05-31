#include "connectfourmanagement.h"

ConnectFourManagement::ConnectFourManagement(QWidget *parent) :
    QWidget(parent)
{
    this->setStyleSheet(Sett::ings().getString("ui/stylesheet"));

    vbMain = new QVBoxLayout(this);
    hbMain = new QHBoxLayout(this);
    wMain = new QWidget(this);

    lbHeading = new QLabel("\t connect4liebmann \t");
    lbHeading->setStyleSheet("QLabel { background-color : black; color : white; font-size:30px; text-align:justified; }");
    wMain->setLayout(hbMain);

    vbMain->addWidget(lbHeading);
    vbMain->addWidget(wMain);

    createLeftColumn();
    hbMain->addWidget(wMainLeft);

    createRightColumn();
    hbMain->addWidget(wMainRight);

    this->setLayout(vbMain);

    indexClientStart();
    netRequestGameList();

    nextHostPort=Sett::ings().getInt("net/myport");
    lePlayerName->setText(Sett::ings().getString("net/playername"));

    settingsWindow = new ConnectFourSettings(this);
    connect(settingsWindow, SIGNAL(playerNameChanged(QString)), this, SLOT(playerNameChanged(QString)));
}



void ConnectFourManagement::createLeftColumn()
{
    //left column
    vbMainLeft = new QVBoxLayout(this);
    wMainLeft = new QWidget(this);
    wMainLeft->setLayout(vbMainLeft);

    //host part
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

    flHost = new QFormLayout();
    btHost = new QPushButton("host that game");
    btHost->setStyleSheet(Sett::ings().getString("ui/st_smallbutton"));
    flHost->addRow(lbPlayerName,lePlayerName);
    flHost->addRow(lbGameName,leGameName);
    flHost->addRow(lbDimX,leDimX);
    flHost->addRow(lbDimY,leDimY);
    flHost->addRow(lbDimZ,leDimZ);
    flHost->addWidget(btHost);
    connect(btHost, SIGNAL(clicked()), this, SLOT(onBtHostClicked()));

    wHost = new QWidget();
    wHost->setLayout(flHost);
    vbMainLeft->addWidget(wHost);

    //Settings/Help/Quit part
    vbSetHelpQuit = new QVBoxLayout(this);
    wSetHepQuit = new QWidget(this);
    btSettings = new QPushButton("settings", this);
    btHelp = new QPushButton("help", this);
    btQuit = new QPushButton("quit", this);
    connect(btSettings, SIGNAL(clicked()), this, SLOT(onBtSettingsClicked()));
    connect(btHelp, SIGNAL(clicked()), this, SLOT(onBtHelpClicked()));
    connect(btQuit, SIGNAL(clicked()), this, SLOT(onBtQuitClicked()));

    vbSetHelpQuit->addWidget(btSettings);
    vbSetHelpQuit->addWidget(btHelp);
    vbSetHelpQuit->addWidget(btQuit);

    wSetHepQuit->setLayout(vbSetHelpQuit);

    vbMainLeft->addWidget(wSetHepQuit);
}


void ConnectFourManagement::createRightColumn()
{
    //right column
    vbMainRight = new QVBoxLayout(this);
    wMainRight = new QWidget(this);
    wMainRight->setLayout(vbMainRight);

    lbIndexGames = new QLabel("games on index server:", this);
    lwIndexGamesList = new QListWidget(this);
    btRefreshIndexGameList = new QPushButton("refresh list");
    btDeleteIndexGame = new QPushButton("delete game");
    btPlayIndexGame = new QPushButton("play game");
    flIndexGames = new QFormLayout(this);
    wIndexGames = new QWidget(this);

    btRefreshIndexGameList->setStyleSheet(Sett::ings().getString("ui/st_smallbutton"));
    btDeleteIndexGame->setStyleSheet(Sett::ings().getString("ui/st_smallbutton"));
    btPlayIndexGame->setStyleSheet(Sett::ings().getString("ui/st_smallbutton"));
    flIndexGames->addRow(lbIndexGames);
    flIndexGames->addRow(lwIndexGamesList);
    flIndexGames->addRow(btRefreshIndexGameList, btDeleteIndexGame);
    flIndexGames->addRow(btPlayIndexGame);
    connect(btRefreshIndexGameList, SIGNAL(clicked()), this, SLOT(netRequestGameList()));
    connect(btDeleteIndexGame, SIGNAL(clicked()), this, SLOT(onBtDeleteGame()));
    connect(btPlayIndexGame, SIGNAL(clicked()), this, SLOT(onBtJoinGame()));

    wIndexGames->setLayout(flIndexGames);


    lbLog = new QLabel("log:", this);
    lwLog = new QListWidget(this);
    vbLog = new QVBoxLayout(this);
    wLog = new QWidget(this);

    vbLog->addWidget(lbLog);
    vbLog->addWidget(lwLog);

    wLog->setLayout(vbLog);



    vbMainRight->addWidget(wIndexGames);
    vbMainRight->addWidget(wLog);
}


void ConnectFourManagement::netRequestGameList()
{
    NetCommand cmd(REQUEST_GAME_LIST);
    indexCom->sendCommand(cmd);
}

void ConnectFourManagement::netDeleteGame(const Game &g)
{
    NetCommand cmd(UNREGISTER_GAME, g.guid);
    indexCom->sendCommand(cmd);
}

void ConnectFourManagement::playerNameChanged(const QString &playerName)
{
    Sett::ings().setString("net/playername", playerName);
    lePlayerName->setText(playerName);
}

void ConnectFourManagement::onBtHostClicked()
{
    QString gameName = leGameName->text();
    QString playerName = lePlayerName->text();
    Sett::ings().setString("net/playername", playerName);

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

            lastToBeHostedOnIndexServer = new Game(true, playerName, gameName, x,y,z, Sett::ings().getString("net/myip"), nextHostPort);
            indexCom->sendCommand(NetCommand(REGISTER_GAME, playerName, gameName, QString::number(x),QString::number(y),QString::number(z), Sett::ings().getString("net/myip"), QString::number(nextHostPort++)));

            //start new GlGame, start listening for connections, and append to glGames list
            //ConnectFourLiebmann *newGlGame = new ConnectFourLiebmann(g, GameJoined);
            //newGlGame->startHostServer();
            //connect(newGlGame, SIGNAL(joinRequestReceived(NetCommand)), this, SLOT(joinRequestReceived(NetCommand)));
            //myHostedGlGames.append(newGlGame);

            logAddEntry(gameName + ": the game is being hosted...");
        }
    }
    else QMessageBox::warning(this, "input error", "your input is invalid. \"game name\" and \"player name\" must be filled out, \"width\", \"height\" and \"depth\" must be numbers and greater than 0!", QMessageBox::Ok);

}

void ConnectFourManagement::joinRequestReceived(const NetCommand &cmd)
{
    if (QMessageBox::question(this, "join request", "the player \"" + cmd.getParameter(0)
                              + "\" wants to join your game \"" + cmd.getParameter(1)
                              + "\". do you want to play?"
                              , QMessageBox::Yes
                              , QMessageBox::No)==QMessageBox::Yes)
    {
        //find game in list

        //send join_game_success

        //show gui
    }
    else
    {

    }
}


void ConnectFourManagement::onBtSettingsClicked()
{
    Sett::ings().setString("net/playername", lePlayerName->text());
    settingsWindow->loadSettingsToGUI();
    settingsWindow->setModal(true);
    settingsWindow->show();
}


void ConnectFourManagement::onBtHelpClicked()
{
    QMessageBox::information(this, "help",
                             "the game is played as followed:\n - move left/right/up/down to move your selector\n - hit space to put down your cube\n - to see hidden cubes, hit shift-key - for deactivating hidden-mode, hit shift-key again", QMessageBox::Ok);
}



void ConnectFourManagement::onBtQuitClicked()
{
    for (int i=0; i<myHostedGames.size(); i++)
    {
        indexCom->sendCommand(NetCommand(UNREGISTER_GAME,myHostedGames.at(i).guid));
    }
    this->close();
}

void ConnectFourManagement::onBtDeleteGame()
{
    int index = lwIndexGamesList->currentIndex().row();

    if (index!=-1)
    {
        Game g = indexGameList.at(index);
        if (g.ip==Sett::ings().getString("net/myip") && g.playerName==Sett::ings().getString("net/playername"))
            netDeleteGame(g);
        else
            QMessageBox::warning(this, "error", "you can only delete games you hosted!");
    }
    else
        QMessageBox::information(this, "info", "you have to select a game in order to delete it!");

}

void ConnectFourManagement::onBtJoinGame()
{
    if (lwIndexGamesList->selectedItems().size()==1)
        {
            Game g;
            bool isGame = false;
            QString game = lwIndexGamesList->selectedItems().at(0)->text();
            int gameIndex = -1;
            for (int i=0; i<indexGameList.size(); i++)
            {
                Game tmpG = indexGameList.at(i);
                QString tmpS = tmpG.toGuiString();
                if (game==tmpS)
                {
                    g = indexGameList.at(i);
                    isGame = true;
                    gameIndex = i;
                }
            }

            if (!g.open)
                QMessageBox::information(this, "error", "this game is not open. you can't join it.");
            else
            {
                //gameToBeJoined = g;
                //emit newGame(gameToBeJoined.playerName, gameToBeJoined.gameName, true, gameToBeJoined.x,gameToBeJoined.y,gameToBeJoined.z, PlayerB);
                //WARNING: emit newGame() does not work after startJoinClient()... threading problem

                ConnectFourLiebmann *newJoinGame = new ConnectFourLiebmann(g, GameJoined);
                myJoinedGlGames.append(newJoinGame);
                newJoinGame->startJoinClient();
                newJoinGame->joinGame();
                newJoinGame->show();

                /*
    mainLayout->removeWidget(lbNoGame);
    if (glGame!=0)
        delete glGame;
    glGame = new ConnectFourGame(this, playerName, gameName, x,y,z, turn);
    glGame->setState(state);
    connect(glGame, SIGNAL(localPlayerMoved(QVector3D)), this, SLOT(localPlayerMoved(QVector3D)));

    mainLayout->addWidget(glGame);
*/

                //startJoinClient(g.ip, g.port);
                //joinCom->sendCommand(NetCommand(JOIN_GAME, Sett::ings().getString("net/playername"), g.gameName, "V1"));
                //gameState = GameJoinInProgress;

                /*if (QMessageBox::Yes == QMessageBox::question(this, "who will start?", "do you want to start? (or let your opponent start)", QMessageBox::Yes, QMessageBox::No))
                    glGame->setTurn(PlayerA);
                else
                    glGame->setTurn(PlayerB);
*/
            }

        }
        else
            QMessageBox::information(this, "error", "you have to select a game", QMessageBox::Ok);

}

void ConnectFourManagement::logAddEntry(const QString &msg)
{
    lwLog->addItem(msg);
}

void ConnectFourManagement::logClear()
{
    lwLog->clear();
}


void ConnectFourManagement::indexClientStart()
{
    indexClientThread = new QThread;
    indexCom = new NetClientCom(Sett::ings().getString("net/indexip"), Sett::ings().getInt("net/indexport"));

    connect(indexClientThread, SIGNAL(started()), indexCom, SLOT(startCommunication()));
    connect(indexClientThread, SIGNAL(finished()), indexClientThread, SLOT(deleteLater()));
    connect(indexCom, SIGNAL(finished()), indexClientThread, SLOT(quit()));
    //connect(indexCom, SIGNAL(finished()), indexCom, SLOT(deleteLater()));
    connect(indexCom, SIGNAL(commandReceived(NetCommand)), this, SLOT(indexClientCommandReceived(NetCommand)), Qt::DirectConnection);
    connect(indexCom, SIGNAL(lostConnection(bool)), this, SLOT(indexClientLostConnection(bool)), Qt::QueuedConnection);
    connect(indexCom, SIGNAL(couldNotConnect()), this, SLOT(indexClientCouldNotConnect()));

    indexCom->moveToThread(indexClientThread);
    indexClientThread->start();
}

void ConnectFourManagement::indexClientCouldNotConnect()
{
    indexClientLostConnection(true);
}

void ConnectFourManagement::indexClientEnd()
{
    indexCom->endCommunication();
    //indexClientThread->quit();
    indexClientThread->terminate();
    indexClientThread->wait();

    delete indexCom;
    delete indexClientThread;
}

void ConnectFourManagement::indexClientCommandReceived(const NetCommand &cmd)
{
    //const QString msg = cmd.getMessage();
    const CommandType type = cmd.getCommandType();

    if (type==REGISTER_SUCCESS)
    {
        lastToBeHostedOnIndexServer->guid = cmd.getParameter(0);
        logAddEntry(lastToBeHostedOnIndexServer->gameName + ": game registred successfully. guid is " + lastToBeHostedOnIndexServer->guid);
        logAddEntry(lastToBeHostedOnIndexServer->gameName + ": waiting for players...");

        myHostedGames.append(Game(lastToBeHostedOnIndexServer->open,
                                   lastToBeHostedOnIndexServer->playerName,
                                   lastToBeHostedOnIndexServer->gameName,
                                   lastToBeHostedOnIndexServer->x,
                                   lastToBeHostedOnIndexServer->y,
                                   lastToBeHostedOnIndexServer->z,
                                   lastToBeHostedOnIndexServer->ip,
                                   lastToBeHostedOnIndexServer->port,
                                   lastToBeHostedOnIndexServer->guid));
        delete lastToBeHostedOnIndexServer;
        lastToBeHostedOnIndexServer=0;
        netRequestGameList();

        lePlayerName->setEnabled(false);
        settingsWindow->lockPlayerNameIPAndPort();

    }
    else if (type==REGISTER_FAILED)
    {
        logAddEntry(lastToBeHostedOnIndexServer->gameName + ": registration failed. message from server: " + cmd.getParameter(0));
    }
    else if (type==ANSWER_GAME_LIST)
    {
        updateGameList(cmd);
    }
    else if (type == UNREGISTER_GAME_FAILED)
    {/*
        QMessageBox::critical(this,
                             "Unregister game failed",
                             "Unregistring the game failed. Message from server:\n" + cmd.getParameter(0) ,
                             QMessageBox::Ok);*/
        logAddEntry("Unregistration of the game failed" + cmd.getParameter(0));
    }
    else if (type== UNREGISTER_GAME_SUCCESS)
    {
        logAddEntry("Game unregistred successfully");
    }

    /*}
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

    cout << "received: " << cmd.toString().toStdString() << endl;*/
}

void ConnectFourManagement::indexClientLostConnection(bool comActive)
{
    if (comActive)
    {
        indexClientEnd();

        if (QMessageBox::question(this, "error",
                                  "lost connection to index server. do you want to try to reconnect?",
                                  QMessageBox::Yes,
                                  QMessageBox::No)==QMessageBox::Yes)
        {
            indexClientStart();
        }
        else
        {
            close();
        }


    }

}


void ConnectFourManagement::updateGameList(const NetCommand &games)
{
    QList<QString> parameters = games.getParameters();
    lwIndexGamesList->clear();

    int count = parameters.at(0).toInt();
    if (parameters.size()>=1 && count>0)
    {
        //todo: guid is not sent with protocol
        //store somewhere the guid's of my games!
        //include them here
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


            bool isMyGame=false;
            for (int i=0; i<myHostedGames.size(); i++)
            {
                Game g = myHostedGames.at(i);

                if (g.gameName==game.gameName && g.playerName==game.playerName)
                    isMyGame=true;
            }

            if (isMyGame)
            {
                QFont fontMyGames;
                fontMyGames.setBold(true);
                fontMyGames.setItalic(true);
                it1->setFont(fontMyGames);
            }

            lwIndexGamesList->insertItem(0, it1);
        }
    }


}

