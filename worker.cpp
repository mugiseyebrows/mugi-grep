#include "worker.h"

Worker::Worker(QObject *parent) :
    QObject(parent)
{
}


void Worker::onSearch(int searchId, QString path, RegExpPath filter, bool notBinary,
                       RegExp search, int linesBefore, int linesAfter, bool cacheFileList)
{
    mCache.add(searchId,path,filter,notBinary,search,linesBefore,linesAfter,cacheFileList);
    emit found(searchId,QString(),-1,0,0,QString());
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
    qDebug() << "onFinishSearch" << id;
    mCache.finish(id);
}
