#ifndef NETCLIENTCOM_H
#define NETCLIENTCOM_H

#include <QObject>
#include <QString>
#include <time.h>
#include <QTcpSocket>
#include <QQueue>
#include <iostream>
#include <QThread>
#include <QStringList>
#include "netcommand.h"

using namespace std;

class NetClientCom : public QObject
{
    Q_OBJECT
private:
    QString host;
    int port;
    QQueue<NetCommand> writeCmd;
    bool comActive;
    bool isconnected = false;
    QTcpSocket *socket;
    //void wait(int ms);
public:
    explicit NetClientCom(QObject *parent = 0);
    NetClientCom(QString host, int port);
    void sendCommand(NetCommand cmd);
    bool isConnected();

signals:
    void commandReceived(const NetCommand &cmd);
    void finished();
    void lostConnection(bool comActive);
    void couldNotConnect();

public slots:
    void startCommunication();
    void endCommunication();

private slots:
    void closeSocket();

};


#endif // NetClientCom_H
