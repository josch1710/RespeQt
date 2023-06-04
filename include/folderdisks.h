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
    QStringList getList() const { return diskList; }

private:
    QDir dir;
    QStringList diskList;
};

#endif // FOLDERDISKS_H
