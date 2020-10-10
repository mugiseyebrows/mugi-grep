#include "worker.h"

#include "searchhits.h"


Worker::Worker(QObject *parent) :
    QObject(parent)
{
}

void Worker::onSearch(SearchParams params)
{
    mCache.add(params);
    SearchHits hits(params.mode(), params.pattern());
    emit found(params.id(), hits);
}

#if 0
void Worker::onPreview(SearchParams params) {
    mCache.add(params);
    SearchHits hits(params.mode(), params.pattern(), params.replacement());
    emit previewed(params.id(), hits);
}
#endif

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
    static int count = 0;
    qDebug() << "search more" << id << count++;
    if (mCache.isFinished(id)) {
        return;
    }
    SearchHits hits = mCache.search(id);
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
