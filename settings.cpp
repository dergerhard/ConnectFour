#include "settings.h"

QSettings Sett::settings(QString("config.ini"), QSettings::IniFormat);
