#include "netclientcom.h"

NetClientCom::NetClientCom(QObject *parent) :
    QObject(parent)
{
    this->host = "127.0.0.1";
    this->port = 8000;
}

NetClientCom::NetClientCom(QString host, int port):comActive(true)
{
    this->host = host;
    this->port = port;
}

void NetClientCom::endCommunication()
{
    comActive = false;
    cout << "Disconnected from " << host.toStdString() << ":" << port << endl;
}

/*
void NetClientCom::wait(int ms)
{
    clock_t begin = clock();
    while ((begin+ms)>clock()) { ; }
}
*/
void NetClientCom::sendCommand(NetCommand cmd)
{
    writeCmd.enqueue(cmd);
    cout << "cmd to send enqueued" << endl;
}

void NetClientCom::closeSocket()
{
    socket->close();
}

void NetClientCom::startCommunication()
{
    bool autoReconnect = true;

    // Connect to Host
    socket=new QTcpSocket;
    socket->connectToHost(host, port);

    connect(socket, SIGNAL(disconnected()), this, SLOT(closeSocket()));

    if (!socket->waitForConnected(5000)) {
        cout << "Error: Could not connect to " << host.toStdString() << ":" << port << endl;
        return;
    }

    cout << "Connected to " << host.toStdString() << ":" << port << endl;
    isconnected=true;

    int i=0;
    while (comActive && socket->isOpen()) {

        if (autoReconnect && (!socket->isOpen()))
        {
            socket->connectToHost(host, port);
            cout << "RECONNECTING..." << endl;

            if (!socket->waitForConnected())
            {
                isconnected=false;
                comActive=false;
                cout << "Error: Could not reconnect to " << host.toStdString() << ":" << port << endl;
            }
        }

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

    isconnected=false;
    socket->disconnect();
    cout << "client: " << "socket->disconnect();" << endl;
    delete socket;
    emit finished();
    emit lostConnection(comActive);
    cout << "client: " << "delete socket, emit finished()" << endl;
}


bool NetClientCom::isConnected()
{
    return isconnected;
}
