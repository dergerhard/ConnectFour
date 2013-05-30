#ifndef NETCOMMAND_H
#define NETCOMMAND_H

#include <QObject>
#include <QQueue>
#include <QMetaType>
#include <QStringList>
#include <iostream>

using namespace std;


//enum to identify the command type
enum CommandType {
    ERROR,
    REGISTER_GAME,
    REGISTER_SUCCESS,
    REGISTER_FAILED,
    REQUEST_GAME_LIST,
    ANSWER_GAME_LIST,
    UNREGISTER_GAME,
    UNREGISTER_GAME_FAILED,
    UNREGISTER_GAME_SUCCESS,
    JOIN_GAME,
    JOIN_GAME_SUCCESS,
    SEAL_GAME,
    JOIN_GAME_FAILED,
    START_GAME,
    MOVE,
    SYNCHRONIZE_GAME_BOARD,
    UPDATE_GAME_BOARD,
    MOVED_FAILED,
    END_GAME,
    ABORT_GAME

};

//list to retreive the corresponding message to CommandType
static const QList<QString> CommandString = QList<QString>()
        << "error: command wrong"
        << "register_game"
        << "register_game_success"      //PROTOCOL ERROR: index msg sould be "register_success"
        << "register_game_failed"       //PROTOCOL ERROR: index msg sould be "register_failed"
        << "request_game_list"
        << "answer_game_list"
        << "unregister_game"
        << "unregister_game_failed"
        << "unregister_game_success"
        << "join_game"
        << "join_game_success"
        << "seal_game"
        << "join_game_failed"
        << "start_game"
        << "move"
        << "synchronize_game_board"
        << "updated_game_board"
        << "moved_failed"
        << "end_game"
        << "abort_game";





class NetCommand
{

private:
    QQueue<QString> parameters;
    CommandType type;

public:
    NetCommand();

    //for creating own commands
    NetCommand(CommandType msg, QString p0="", QString p1="", QString p2="", QString p3="", QString p4="", QString p5=""
            , QString p6="", QString p7="", QString p8="", QString p9="");

    //for receiving commands
    NetCommand(QString messageWithParameters);
    NetCommand(const NetCommand &cSource);
    ~NetCommand();

    void addParameter(QString param);
    QQueue<QString> getParameters() const;
    QString getParameter(int i=1) const;

    QString toString(bool debug=false) const;
    //void debugOutput();
    QString getMessage() const;
    CommandType getCommandType() const;

};

Q_DECLARE_METATYPE(NetCommand)

#endif // NETCOMMAND_H
