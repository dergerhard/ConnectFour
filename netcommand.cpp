#include "netcommand.h"

NetCommand::NetCommand()
{
}

NetCommand::~NetCommand()
{

}

NetCommand::NetCommand(CommandType msg, QString p0, QString p1, QString p2, QString p3, QString p4, QString p5, QString p6, QString p7, QString p8, QString p9)
{
    type = msg;
    if (p0 != "") parameters.enqueue(p0);
    if (p1 != "") parameters.enqueue(p1);
    if (p2 != "") parameters.enqueue(p2);
    if (p3 != "") parameters.enqueue(p3);
    if (p4 != "") parameters.enqueue(p4);
    if (p5 != "") parameters.enqueue(p5);
    if (p6 != "") parameters.enqueue(p6);
    if (p7 != "") parameters.enqueue(p7);
    if (p8 != "") parameters.enqueue(p8);
    if (p9 != "") parameters.enqueue(p9);
}


NetCommand::NetCommand(QString messageWithParameters)
{
    QStringList parts = messageWithParameters.split(";");
    if (CommandString.contains(parts.at(0).trimmed()))
            type = static_cast<CommandType>(CommandString.indexOf(parts.at(0)));
    for (int i=1; i<parts.size(); i++)
        parameters.enqueue(parts.at(i));
}


NetCommand::NetCommand(const NetCommand &cSource)
{
    this->type=cSource.type;
    for (int i=0; i<cSource.parameters.count(); i++)
        this->parameters.enqueue(cSource.parameters.at(i));
}

void NetCommand::addParameter(QString param)
{
    parameters.enqueue(param);
}

QQueue<QString> NetCommand::getParameters() const
{
    return parameters;
}

QString NetCommand::toString(bool debug) const
{
    QString cmd = CommandString.at(type);
    for (int i=0; i<parameters.size(); i++)
        cmd.append(";").append(parameters.at(i));
    return cmd;
}

/*
void NetCommand::debugOutput()
{
    cout << "Message: " << message.toStdString() << endl;
    for (int i=0; i<parameters.size(); i++)
       cout << "  Parameter " << i << ": " << parameters.at(i).toStdString() << endl;
}*/

QString NetCommand::getMessage() const
{
    return CommandString.at(type);
}

QString NetCommand::getParameter(int i) const
{
    if (i<parameters.size())
        return parameters.at(i);
    return "";
}

CommandType NetCommand::getCommandType() const
{
    return type;
}
