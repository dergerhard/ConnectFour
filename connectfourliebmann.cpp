#include "connectfourliebmann.h"

ConnectFourLiebmann::ConnectFourLiebmann(Game g, GameState state) : game(g), gameState(state)
{

    Sett::ings().setString("ui/stylesheet", "QWidget{background-color:black; color:white;}QPushButton {background-color: rgb(72,27,250); border-style: outset;border-width: 2px;border-radius: 10px;border-color: beige;font: bold 12px;min-width: 7em;min-height: 2em;padding: 6px;} QPushButton:hover {background-color: red;} QPushButton:pressed {background-color: rgb(170, 0, 0);} QPushButton[enabled=\"false\"] {background-color: rgb(170, 170, 170);}");
    this->setStyleSheet(Sett::ings().getString("ui/stylesheet"));

    mainLayout = new QHBoxLayout;
    mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);

    //setStyleSheet("QPushButton{min-width:100px;background-color:black;color:white;}");
    navigation = new QStackedLayout(this);


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


    setLayout(mainLayout);
    setWindowTitle(tr("Connect Four Liebmann"));

    //set up index server connection
    qRegisterMetaType<NetCommand>("NetCommand");

    connect(this, SIGNAL(sEndGame(QString,bool)), this, SLOT(endGame(QString,bool)), Qt::QueuedConnection);

    newGame(game, state); //TODO
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
    addGameInProgressStatus(QString("your opponent moved to ")
                            .append(QString::number((int)move.x())).append("/")
                            .append(QString::number((int)move.y())).append("/")
                            .append(QString::number((int)move.z())));


/*
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
        hostCom->sendCommand(cmd);*/
}



void ConnectFourLiebmann::newGame(Game g, bool state)
{
    if (glGame!=0)
        delete glGame;
    glGame = new ConnectFourGame(this, g.playerName, g.gameName, g.x, g.y, g.z, PlayerB);
    glGame->setState(state);
    connect(glGame, SIGNAL(localPlayerMoved(QVector3D)), this, SLOT(localPlayerMoved(QVector3D)));

    mainLayout->addWidget(glGame);
}


void ConnectFourLiebmann::endGame(QString msg, bool yesno)
{
    /*int res;
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


        if (gameState==JoinedGameInProgress)
            endJoinClient();
        else endHostServer();

        gameState = NoGame;
    }
    */
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



void ConnectFourLiebmann::addGameInProgressStatus(QString msg)
{
    lwGameInProgressStatus->addItem(new QListWidgetItem(msg));
}

void ConnectFourLiebmann::clearGameInProgressStatus()
{
    lwGameInProgressStatus->clear();
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

    switch (cmd.getCommandType())
    {
    case JOIN_GAME: emit joinRequestReceived(cmd); break;
    case MOVE: break;
    case ABORT_GAME: break;
    }
}




void ConnectFourLiebmann::startJoinClient()
{
    joinClientThread = new QThread;
    joinCom = new NetClientCom(game.ip, game.port);

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

    cout << "joinedGameCommandReceived: " << cmd.toString().toStdString() << endl;
    switch (cmd.getCommandType())
    {

    case JOIN_GAME_SUCCESS:
    {
        if (cmd.getParameter(0).toUpper()=="V1")
            protocol= V1;
        else protocol = V2;
        cout << "I AM IN joinedGameCommandReceived" << endl;
        gameState =JoinedGameInProgress;
        addGameInProgressStatus(QString("joined the game \"")
                                .append(gameToBeJoined.gameName).append("\" with \"").append(gameToBeJoined.playerName).append("\" successfully!"));
    } break;

    case JOIN_GAME_FAILED:
    {
        QMessageBox::information(this, "error", QString("could not join the game. the server says: ").append(cmd.getParameter(2)));
    } break;

    case START_GAME:
    {
        glGame->setTurn(PlayerA);
    } break;
    case MOVE:
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
    } break;

    case SYNCHRONIZE_GAME_BOARD:
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
        {//error in turn of yncGameBoard(states)
            emit sEndGame("board synchronisation error. the game will be closed", false);
            hostCom->sendCommand(NetCommand(MOVED_FAILED, "synchronization didn't work"));
        }
    } break;

    case UPDATE_GAME_BOARD:
    {
        int moveid = cmd.getParameter(0).toInt();
        int state = cmd.getParameter(1).toInt();
        //TODO
    } break;

    case MOVED_FAILED:
    {
        emit sEndGame(QString("one of your moves failed - the game will be closed. the host says that is wrong:").append(cmd.getParameter(0)), false);
    } break;

    case END_GAME:
    {
        QString msg;
        if (cmd.getParameter(0).toInt()==0)
            msg = "game over! you reached a draw! no one won, or both ;)";
        else if (cmd.getParameter(0).toInt()==1)
            msg = "game over! you lost!";
        if (cmd.getParameter(0).toInt()==2)
            msg = "congratulations! you won!";
        emit sEndGame(msg, false);
    } break;

    case ABORT_GAME:
    {
        emit sEndGame(QString("your opponent quit the game. his reason:").append(cmd.getParameter(0)), false);
    } break;

    }

}



void ConnectFourLiebmann::joinGame()
{
    joinCom->sendCommand(NetCommand(JOIN_GAME, Sett::ings().getString("net/playername"), game.gameName, "V1"));
}
