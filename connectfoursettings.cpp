#include "connectfoursettings.h"
#include <iostream>

ConnectFourSettings::ConnectFourSettings(QWidget *parent) :
    QDialog(parent)
{
    layout = new QFormLayout(this);

    lbPlayerName = new QLabel();
    lbPlayerName->setText("player name:");
    edPlayerName = new QLineEdit();

    lbGameName = new QLabel();
    lbGameName->setText("game name:");
    edGameName = new QLineEdit;

    lbMyIP = new QLabel();
    lbMyIP->setText("my ip:");
    cbMyIP = new QComboBox();

    lbMyPort = new QLabel();
    lbMyPort->setText("my port:");
    edMyPort = new QLineEdit;

    lbIndexServerIP = new QLabel();
    lbIndexServerIP->setText("index server ip:");
    edIndexServerIP = new QLineEdit;

    lbIndexServerPort = new QLabel();
    lbIndexServerPort->setText("index server port:");
    edIndexServerPort = new QLineEdit;

    btCancel = new QPushButton("cancel");
    btSave = new QPushButton("save");

    layout->addRow(lbPlayerName, edPlayerName);
    layout->addRow(lbGameName, edGameName);
    layout->addRow(lbMyIP, cbMyIP);
    layout->addRow(lbMyPort, edMyPort);
    layout->addRow(lbIndexServerIP, edIndexServerIP);
    layout->addRow(lbIndexServerPort, edIndexServerPort);
    layout->addRow(btCancel, btSave);

    this->setLayout(layout);


    QNetworkInterface *inter = new QNetworkInterface();
    QList<QHostAddress> list;
    list=inter->allAddresses();


    for (int i = 0; i < list.size(); ++i)
        if (list.at(i).toString().contains("."))
            cbMyIP->addItem(list.at(i).toString());

    connect(btCancel, SIGNAL(clicked()), this, SLOT(onBtCancelClicked()));
    connect(btSave, SIGNAL(clicked()), this, SLOT(onBtSaveClicked()));

    edPlayerName->setText(Sett::ings().getString("net/playername"));
    edGameName->setText(Sett::ings().getString("net/gamename"));
    edMyPort->setText(QString::number(Sett::ings().getInt("net/myport")));

    edIndexServerIP->setText(Sett::ings().getString("net/indexip"));
    edIndexServerPort->setText(QString::number(Sett::ings().getInt("net/indexport")));

    int selectIndex = -1;
    for (int i=0; i<cbMyIP->count(); i++)
        if (cbMyIP->itemText(i)==Sett::ings().getString("net/myip"))
            selectIndex=i;
    if (selectIndex==-1)
        selectIndex=0;
    cbMyIP->setCurrentIndex(selectIndex);
}


void ConnectFourSettings::onBtCancelClicked()
{
    this->close();
}

void ConnectFourSettings::onBtSaveClicked()
{
    QString pName = edPlayerName->text();
    QString gName = edGameName->text();
    QString sPort = edMyPort->text();
    QString ip = cbMyIP->currentText();

    QString indexIP = edIndexServerIP->text();
    QString indexPort = edIndexServerPort->text();

    std::cout << indexIP.toStdString() << endl;
    bool portIsInt;
    bool indexPortIsInt;
    int port = sPort.toInt(&portIsInt);
    int iPort = indexPort.toInt(&indexPortIsInt);

    bool canSaveIt=true;
    if (portIsInt==false || indexPortIsInt==false)
    {
        canSaveIt=false;
        QMessageBox::information(this, "error", "the ports must be an integers!", QMessageBox::Ok);
    }
    else if (pName.size()<=0 || gName.size()<=0)
    {
        QMessageBox::information(this, "error", "playername and gamename must be filled out!", QMessageBox::Ok);
    }
    else if (!indexIP.contains(QRegExp("\\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b")))
    {
        QMessageBox::information(this, "error", "the index server ip is incorrect! pleas provide a valid one!", QMessageBox::Ok);
    }
    else
    {
        Sett::ings().setString("net/playername", pName);
        Sett::ings().setString("net/gamename", gName);
        Sett::ings().setString("net/myip", ip);
        Sett::ings().setInt("net/myport", port);
        Sett::ings().setString("net/indexip", indexIP);
        Sett::ings().setInt("net/indexport", iPort);
        this->close();
    }


}
