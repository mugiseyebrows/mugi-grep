#include "worker.h"

Worker::Worker(QObject *parent) :
    QObject(parent)
{
}


void Worker::onSearch(int searchId,QString path, RegExpPath filter, bool notBinary,
                       RegExp search, int linesBefore, int linesAfter)
{
    mCache.add(searchId,path,filter,notBinary,search,linesBefore,linesAfter);
    emit found(searchId,QString(),-1,0,0,QString());
}

void Worker::onSearchMore(int id)
{
    //qDebug() << "Worker2::onSearchMore";
    QString data;
    QString file;
    int complete;
    int total;
    int filtered;
    mCache.search(id,data,&complete,&total,&filtered,file);
    emit found(id,data,complete,total,filtered,file);
}

void Worker::onFinishSearch(int id)
{
    mCache.finish(id);
}

#if 0
void Worker2::onCalcSize()
{

    QString key;

    QMap<QString,int> fileSize = mCache.fileSize();

    int size = 0;
    int keysSize = 0;
    QStringList keys = fileSize.keys();
    foreach(key,keys) {
        keysSize += key.size();
        size += fileSize[key];
    }

    //qDebug() << size << keysSize;

    emit sizeCalculated(size,keysSize);
}
#endif


#if 0
QMap<QString, int> SearchCache::fileSize() const
{
    return mFileSize;
}
#endif
