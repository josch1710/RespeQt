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

struct DiskLabel
{
    DiskLabel(const QString& _title = QString(), const QString& _diskNo = QString(), bool _sideB = false)
            : title(_title), diskNo(_diskNo), sideB(_sideB) { }
    void clear() {title.clear(); diskNo.clear(); sideB = false;}
    QString title;
    QString diskNo;
    bool sideB;
};

struct FloppyArt
{
    FloppyArt() { }
    void clear() { pic.clear(); label.clear(); }
    QString pic;
    DiskLabel label;
};

#if 0
class FolderSettings : public QSettings
{
public:
    explicit FolderSettings(const QString& path);
    virtual ~FolderSettings();

    void load();
    void save();

private:
    QString _path;
};
#endif

class FolderDisks : public QObject
{
    Q_OBJECT

public:
    FolderDisks();
    bool load(const QString& path);
    int count() const { return diskList.count(); }
    QStringList disks() const { return diskList; }
    QStringList folders() const { return dirList; }
/*  QString defaultPic() const { return _defaultPic; }          TBD: omit...
    const FloppyArt& diskArt(int i) { return _diskArt.at(i); }
    QString diskPic(int i) { return diskArt(i).pic; }
    QString diskTitle(int i) { return diskArt(i).label.title; }
    QString indexLabel(int i);
    FolderSettings& getSettings() { return *_settings; }
*/
private:
    QDir dir;
    QStringList dirList;
    QStringList diskList;
    QString          _defaultPic;
    QList<FloppyArt> _diskArt;
//  FolderSettings*  _settings = nullptr;
};

#endif // FOLDERDISKS_H
