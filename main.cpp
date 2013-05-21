#include <QApplication>
#include "connectfourliebmann.h"
//#include "connectfournavigationwidget.h"
#include <iostream>
#include <string>
#include "netclientcom.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConnectFourLiebmann w;

    w.show();
    //w.showFullScreen();
    
    //ConnectFourNavigationWidget c4w;
    //c4w.show();

    return a.exec();




}
