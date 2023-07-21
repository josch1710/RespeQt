/*
 * folderdisks.h - declaration of DiskBrowserDialog's FolderDisks class.
 *
 * Copyright 2023 D.Caputi
 */
#ifndef FOLDERDISKS_H
#define FOLDERDISKS_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QSettings>

struct FloppyArt
{
    QString pic;
    QString title;
    int  index;
    bool sideB;
};

class FolderSettings : public QSettings
{
public:
    explicit FolderSettings(const QString& path);
    virtual ~FolderSettings();

    void load();
    void save();
};

class FolderDisks : public QObject
{
    Q_OBJECT

public:
    FolderDisks();
    bool load(const QString& path);
    int count() const { return diskList.count(); }
    QStringList disks() const { return diskList; }
    QStringList folders() const { return dirList; }

private:
    QDir dir;
    QStringList dirList;
    QStringList diskList;
    QString          _defaultPic;
    QList<FloppyArt> _diskArt;
    FolderSettings*  _settings;
};

#endif // FOLDERDISKS_H
