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
};

#endif // FOLDERDISKS_H
