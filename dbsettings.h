#ifndef DBSETTINGS_H
#define DBSETTINGS_H

#include <QSettings>

class DbSettings
{
public:
    explicit DbSettings();

    void setDefaultPic(const QString& defPic);

private:
    QSettings* _settings = nullptr;
    bool _useAppSettings = false;
};

#endif // DBSETTINGS_H
