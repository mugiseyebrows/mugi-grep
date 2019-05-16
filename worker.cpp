#include "worker.h"

Worker::Worker(QObject *parent) :
    QObject(parent)
{
}


void Worker::onSearch(int action, int searchId, QString path, RegExpPath filter, bool notBinary,
                       RegExp search, int linesBefore, int linesAfter, bool cacheFileList, QString relpacement)
{
    mCache.add(action, searchId, path, filter, notBinary,search,linesBefore,linesAfter,cacheFileList, relpacement);
    emit found(searchId,QString(),-1,0,0,QString());
}

void Worker::onReplace(int searchId)
{
    mCache.replace(searchId);
}

void Worker::onCountMatchedFiles(QString path, RegExpPath filter, bool notBinary) {
    QPair<int,int> fileCount = mCache.countMatchedFiles(path,filter,notBinary);
    emit count(fileCount.first, fileCount.second);
}

void Worker::onGetAllFiles(QString path)
{
    QStringList files = mCache.getAllFiles(path,true);
    emit allFiles(path,files);
}

void Worker::onSearchMore(int id)
{
    //qDebug() << "Worker::onSearchMore";
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
    emit canReplace(mCache.isPreview(id));
    mCache.finish(id);
}
