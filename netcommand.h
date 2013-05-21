#ifndef NETCOMMAND_H
#define NETCOMMAND_H

#include <QObject>
#include <QQueue>
#include <QMetaType>
#include <QStringList>
#include <iostream>

using namespace std;

class NetCommand
{

private:
    QString message;
    QQueue<QString> parameters;
    bool awaitAnswer;

public:
    NetCommand();
    NetCommand(QString message, bool awaitAnswer=false, QString p0="", QString p1="", QString p2="", QString p3="", QString p4="", QString p5=""
            , QString p6="", QString p7="", QString p8="", QString p9="");

    //NetCommand(QString messageWithParameters, bool awaitAns=false);
    NetCommand(const NetCommand &cSource);
    ~NetCommand();

    void addParameter(QString param);
    QQueue<QString> getParameters() const;
    QString getParameter(int i=1) const;

    QString toString(bool debug=false) const;
    void debugOutput();
    QString getMessage() const;

};

Q_DECLARE_METATYPE(NetCommand)

#endif // NETCOMMAND_H
