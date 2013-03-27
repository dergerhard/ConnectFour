#ifndef CONNECTFOURLIEBMANN_H
#define CONNECTFOURLIEBMANN_H

#include <QMainWindow>
#include <QtGui>
#include "connectfourwidget.h"

class ConnectFourLiebmann : public QWidget
{
    Q_OBJECT
    
public:
    ConnectFourLiebmann();
    ~ConnectFourLiebmann();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    QSlider *createSlider();

    QHBoxLayout *mainLayout;
    ConnectFourWidget *glWidget;
    QSlider *xSlider;
    QSlider *ySlider;
    QSlider *zSlider;
    QPushButton *btDo;
    QLineEdit *edX, *edY, *edZ;
    QPushButton *camIsometric;
    QPushButton *camLeft;
    QPushButton *camRight;


public slots:
    void onBtDoClicked();
};

#endif
