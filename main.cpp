#include <QApplication>
#include "connectfourliebmann.h"
//#include "connectfournavigationwidget.h"
#include <iostream>
#include <string>
#include "netclientcom.h"
#include "connectfourmanagement.h"

using namespace std;






int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //ConnectFourLiebmann w;
    //w.show();

    ConnectFourManagement c4m;
    c4m.show();


    return a.exec();




}
