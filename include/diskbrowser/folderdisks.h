/*
 * folderdisks.h - declaration of DiskBrowserDialog's FolderDisks class.
 *
 * Copyright 2023 D.Caputi
 */
#ifndef FOLDERDISKS_H
#define FOLDERDISKS_H

#include <QDir>
#include <QSettings>
#include <utility>

struct DiskLabel
{
    explicit DiskLabel(QString  _title = QString(), QString  _index = QString(), const bool _sideB = false)
            : title(std::move(_title)), index(std::move(_index)), sideB(_sideB) { }

    void clear() { title.clear(); index.clear(); sideB = false; }

    [[nodiscard]] bool isEmpty() const { return title.isEmpty() && index.isEmpty() && !sideB; }

    QString title;
    QString index;
    bool sideB;
};

struct FloppyArt
{
    FloppyArt() = default;

    void clear() { pic.clear(); label.clear(); }
    [[nodiscard]] bool isEmpty() const { return pic.isEmpty() && label.isEmpty(); }

    QString pic;
    DiskLabel label;
};


class FolderDisks : public QObject
{
    Q_OBJECT

public:
    FolderDisks();

    bool load(const QString& path);
    [[nodiscard]] int count() const { return diskList.count(); }
    [[nodiscard]] QStringList disks() const { return diskList; }
    [[nodiscard]] QStringList folders() const { return dirList; }

private:
    QDir dir;
    QStringList dirList;
    QStringList diskList;
};

#endif // FOLDERDISKS_H
