#include "worker.h"

Worker::Worker(QObject *parent) :
    QObject(parent)
{
}

void Worker::onSearch(SearchParams params)
{
    mCache.add(params);
    emit found(params.id(),QString(),-1,0,0,QString());
}

void Worker::onReplace(int searchId)
{
    int files,lines;
    QStringList notChanged;
    mCache.replace(searchId, &files, &lines, notChanged);
    emit replaced(searchId, files, lines, notChanged);
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

void Worker::onFinishSearch(int id) {
    emit canReplace(id, mCache.isPreview(id));
    //mCache.finish(id);
}

void Worker::onCanReplace(int id) {
    emit canReplace(id, mCache.isPreview(id));
}
