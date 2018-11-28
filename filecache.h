#ifndef FILECACHE_H
#define FILECACHE_H


#include <QByteArray>
#include <QString>

#include <QDateTime>
#include <QMutex>
#include <QMap>
#include <QSet>

#include "3rdparty/orderedmap.h"

class FileEntry {
public:
    FileEntry() {}
    FileEntry(const QDateTime& date, const QByteArray& data) : date(date), data(data) {}
    QDateTime date;
    QByteArray data;
};

class FileCache
{
public:
    static FileCache* instance();

    QByteArray file(const QString &path, bool skipBinary, bool *binary, bool *readOk);

protected:

    QByteArray fileData(const QString &path, bool skipBinary, bool *binary, bool *readOk);
    void checkCacheLimit();

    OrderedMap<QString,FileEntry> mCache;
    static FileCache* mInstance;
    FileCache();
    QMutex mMutex;
};

#endif // FILECACHE_H
