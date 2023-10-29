#include "dbsettings.h"
#include "respeqtsettings.h"
#include <QStandardPaths>
#include <QDir>

DbSettings::DbSettings()
{
    auto    locType = QStandardPaths::AppDataLocation;
    QString appFolder = QStandardPaths::writableLocation(locType);
    QDir appDataDir(appFolder);

    if (!appDataDir.exists())
        appDataDir.mkpath(".");

    QString file = appDataDir.absoluteFilePath("dbSettings.ini");
    _settings = new QSettings(file, QSettings::IniFormat);

    bool ok = (_settings && _settings->isWritable() && _settings->status() == QSettings::NoError);
    if (!ok)
    {
        // TBD: overkill? maybe just error popup (but how?)
        _settings = RespeqtSettings::instance()->mSettings;
        _useAppSettings = true;
    }
}

void DbSettings::setDefaultPic(const QString& defPic)
{
    _settings->setValue("db/default_pic", defPic);
}
