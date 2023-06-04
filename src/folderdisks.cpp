#include "folderdisks.h"
#include <QDir>

FolderDisks::FolderDisks() : QObject()
{
}

bool FolderDisks::load(const QString& path)
{
    QString targetPath = path.isEmpty() ? QDir::currentPath() : path;

    if (dir.exists(targetPath))
    {
        dir.setPath(targetPath);

        QStringList filter = QStringList() << "*.atr" << "*.ATR" << "*.pro" << "*.PRO";
        diskList = dir.entryList(filter, QDir::Files);
    }

    return !diskList.isEmpty();
}
