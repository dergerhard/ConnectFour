#include "netservercom.h"

NetServerCom::NetServerCom(QObject *parent) :
    QObject(parent)
{

}

void NetServerCom::endCommunication()
{
    comActive = false;
    cout << "Server connection disconnected" << endl;
}


void NetServerCom::sendCommand(NetCommand cmd)
{
    writeCmd.enqueue(cmd);
    cout << "cmd to send enqueued" << endl;
}

void NetServerCom::startListening()
{
    QString ip = Sett::ings().getString("net/myip");
    int port = Sett::ings().getInt("net/mynextfreeport");
    Sett::ings().setInt("net/mynextfreeport", port+1);

    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(startCommunication()));

    if (!server->listen(QHostAddress(ip),port))
    {
        return;
    }
}

void NetServerCom::closeSocket()
{
    socket->close();
}


void NetServerCom::startCommunication()
{
    socket = server->nextPendingConnection();

    connect(socket, SIGNAL(disconnected()), this, SLOT(closeSocket()));

    while (comActive && socket->isOpen())
    {
        //read queued commands and send to host
        if (writeCmd.size()>0)
        {
            NetCommand cmd = writeCmd.dequeue();
            QString m = cmd.toString().append("\n");
            socket->write(m.toAscii());
            socket->flush();
            cout << "queued cmd sent: " << cmd.toString().toStdString() << endl;
        }

        //read command from host
        if (socket->waitForReadyRead(100))
        {
            QByteArray data = socket->readAll();
            NetCommand cmd = NetCommand(QString(data.constData()));
            emit commandReceived(cmd);
        }
    }


    socket->disconnect();
    delete socket;
    emit finished();
    emit lostConnection(comActive);
}
