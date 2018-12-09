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
    mCache.finish(id);
}
