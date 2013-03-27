#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtGlobal>
#include <QColor>

class Settings
{
private:
    Settings();
public:
    static const qreal CubeSize = 0.1;      // lenght of the edge of a cube
    static const qreal EdgeFactor = 1.2; // the risize-factor for the field for one cubes (if that is 1, there will be no space between cubes)
    static const qreal ColumnWidth =0.005; // width of the orientational column
    //static const QColor ColumnColor(255,255,255,100);
    static const qreal SCALE = 0.001;

    //Helper animation settings (vertical bar)
    static const int HAnimTime = 400;
    static const int HAnimPause = 2000;
    static const int HAnimStep = 10;

};

#endif // SETTINGS_H
