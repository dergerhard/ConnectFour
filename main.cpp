#include <QApplication>
#include "connectfourliebmann.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConnectFourLiebmann w;
    w.show();
    //w.showFullScreen();
    
    return a.exec();
}
