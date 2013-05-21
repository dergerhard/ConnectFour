#ifndef NETSERVERCOM_H
#define NETSERVERCOM_H

#include <QObject>
#include <QString>
#include <time.h>
#include <QTcpServer>
#include <QTcpSocket>
#include <QQueue>
#include <iostream>
#include <QThread>
#include <QStringList>
#include <QUdpSocket>

#include "netcommand.h"
#include "settings.h"

using namespace std;

class NetServerCom : public QObject
{
    Q_OBJECT
private:
    QTcpServer *server;
    QQueue<NetCommand> writeCmd;
    bool comActive=true;
    QTcpSocket *socket;

public:
    explicit NetServerCom(QObject *parent = 0);
    //NetServerCom();
    void sendCommand(NetCommand cmd);

signals:
    void commandReceived(const NetCommand &cmd);
    void finished();
    void lostConnection(bool comActive);

public slots:
    void startListening();
    void startCommunication();
    void endCommunication();
    void closeSocket();
};


#endif // NetServerCom_H
