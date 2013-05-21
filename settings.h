#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtGlobal>
#include <QColor>
#include <QSettings>
#include <QString>


class Sett{
private:
    static QSettings settings;
    Sett() {  }
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    Sett(Sett const&);              // Don't Implement
    void operator=(Sett const&); // Don't implement

public:
    static Sett& ings()
    {
        static Sett    instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
        return instance;
    }

    static int getInt(const QString &val) { return settings.value(val).toInt(); }
    static qreal getReal(const QString &val) { return settings.value(val).toReal(); }
    static QString getString(const QString &val) { return settings.value(val).toString();}

    static void setInt(const QString &key, int val) { settings.setValue(key, val); }
    static void setReal(const QString &key, qreal val) { settings.setValue(key, val); }
    static void setString(const QString &key, const QString &val) { settings.setValue(key, val); }
};


#endif // SETTINGS_H
