#ifndef CONNECTFOURSETTINGS_H
#define CONNECTFOURSETTINGS_H

#include <QDialog>
#include <QWidget>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QNetworkInterface>
#include <QMessageBox>

#include "settings.h"

class ConnectFourSettings : public QDialog
{
    Q_OBJECT

    QFormLayout *layout;
    QLabel *lbPlayerName;
    QLineEdit *edPlayerName;

    QLabel *lbGameName;
    QLineEdit *edGameName;

    QLabel *lbMyIP;
    QComboBox *cbMyIP;

    QLabel *lbMyPort;
    QLineEdit *edMyPort;

    QLabel *lbIndexServerIP;
    QLineEdit *edIndexServerIP;

    QLabel *lbIndexServerPort;
    QLineEdit *edIndexServerPort;

    QPushButton *btSave;
    QPushButton *btCancel;

public:
    explicit ConnectFourSettings(QWidget *parent = 0);
    
signals:
    
public slots:

    void onBtSaveClicked();
    void onBtCancelClicked();
    
};

#endif // CONNECTFOURSETTINGS_H
