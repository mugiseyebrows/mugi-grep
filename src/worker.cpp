#include "worker.h"

#include "searchhits.h"

Worker::Worker(QObject *parent) :
    QObject(parent)
{
}

void Worker::onSearch(SearchParams params)
{
    mCache.add(params);
    SearchHits hits = SearchHits();
    hits.setPattern(params.pattern());
    emit found(params.id(), hits);
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
    //qDebug() << "Worker::onGetAllFiles" << path;
    QStringList files = mCache.getAllFiles(path,true);
    emit allFiles(path,files);
}

void Worker::onSearchMore(int id)
{
    //qDebug() << "Worker::onSearchMore";
    /*QString data;
    QString file;*/



    static int count = 0;

    qDebug() << "search more" << id << count++;

    if (mCache.isFinished(id)) {
        return;
    }

    SearchHits hits;
    mCache.search(id,hits);
    emit found(id,hits);
    if (mCache.isFinished(id)) {
        mCache.finish(id);
    }
}

void Worker::onFinishSearch(int id) {
    //emit canReplace(id, mCache.isPreview(id));
    //mCache.finish(id);
}

void Worker::onCanReplace(int id) {
    //emit canReplace(id, mCache.isPreview(id));
}
