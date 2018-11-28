#include "filecache.h"

#include <QFile>
#include <QTextCodec>
#include <QDebug>
#include <QFileInfo>

FileCache *FileCache::mInstance = nullptr;

FileCache *FileCache::instance()
{
    if (!mInstance) {
        mInstance = new FileCache();
    }
    return mInstance;
}



void FileCache::checkCacheLimit() {

    QList<QString> keys = mCache.keys();
    QList<int> sizes;
    for(const QString& key: keys) {
        sizes << mCache[key].data.size();
    }

    int sum = std::accumulate(sizes.begin(),sizes.end(),0);

    int limit = 1024 * 1024 * 10;

    //limit = 1024 * 100;

    while (sum > limit) {
        //qDebug() << "cache size" << sum;
        QString key = keys.takeFirst();
        mCache.remove(key);
        sum -= sizes.takeFirst();
        //qDebug() << "removing" << key << "from cache";
    }
    //qDebug() << "cache size" << sum;

}

QByteArray FileCache::file(const QString &path, bool skipBinary, bool* binary, bool* readOk)
{
    QMutexLocker locker(&mMutex);

    QDateTime date = QFileInfo(path).lastModified();

    if (!mCache.contains(path)) {
        QByteArray data = fileData(path,skipBinary,binary,readOk);
        FileEntry entry(date,data);
        mCache[path] = entry;
        //qDebug() << "adding" << path << "to cache";
        checkCacheLimit();
        return entry.data;
    }

    FileEntry entry = mCache[path];
    if (date > entry.date) {
        QByteArray data = fileData(path,skipBinary,binary,readOk);
        entry.date = date;
        entry.data = data;
        mCache[path] = entry;
        checkCacheLimit();
        return entry.data;
    }

    return entry.data;

}

QByteArray FileCache::fileData(const QString& path, bool skipBinary, bool* binary, bool* readOk) {

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        if (readOk) {
            *readOk = false;
        }
    }
    if (readOk) {
        *readOk = true;
    }

    if (binary)
        *binary = false;

    if (skipBinary) {
        QByteArray dataSample = file.read(2 * 1024);
        if (dataSample.indexOf('\0') > -1) {
            //qDebug() << path << " is binary";
            if (skipBinary) {
                file.close();
                if (binary)
                    *binary = true;
                return QByteArray();
            }
        } else {
            //qDebug() << path << " is not binary";
        }
        file.seek(0);
    }

    QByteArray res = file.readAll();
    return res;
}

FileCache::FileCache()
{

}
