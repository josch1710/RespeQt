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
    DiskLabel(const QString& _title = QString(), const QString& _index = QString(), bool _sideB = false)
            : title(_title), index(_index), sideB(_sideB) { }

    void clear() { title.clear(); index.clear(); sideB = false; }

    bool isEmpty() const { return title.isEmpty() && index.isEmpty() && !sideB; }

    QString title;
    QString index;
    bool sideB;
};

struct FloppyArt
{
    FloppyArt() { }

    void clear() { pic.clear(); label.clear(); }
    bool isEmpty() const { return pic.isEmpty() && label.isEmpty(); }

    QString pic;
    DiskLabel label;
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
};

#endif // FOLDERDISKS_H
