#include "worker.h"

#include "searchhits.h"
#include "countfilesparams.h"

#include <QTimer>

Worker::Worker(QObject *parent) :
    QObject(parent)
{
}

void Worker::onSearch(SearchParams params)
{
    mCache.add(params);
    SearchHits hits(params.pattern());
    emit found(params.id(), hits);
}

void Worker::onCountMatchedFiles(QString path, RegExpPath filter) {
    QPair<int,int> fileCount = mCache.countMatchedFiles(path,filter);
    emit count(fileCount.first, fileCount.second);
}



void Worker::onCountFiles(CountFilesParams params) {
    QPair<int,int> count = mCache.countMatchedFiles(params.path(), params.filter());
    params.setFiltered(count.first);
    params.setTotal(count.second);
    emit filesCounted(params);
}

void Worker::onGetAllFiles(QString path)
{
    //qDebug() << "Worker::onGetAllFiles" << path;
    QStringList files = mCache.getAllFiles(path,true);
    emit allFiles(path,files);
}

void Worker::onGetListing(GetListingParams params)
{
    QStringList files = mCache.getAllFiles(params.path(),params.cacheFileList());
    emit listing(params.path(),files);
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

#include "fileio.h"

void Worker::onReplace(ReplaceParams params)
{
    QList<ReplaceFile> files = params.files();
    int countLines = 0;
    int countFiles = 0;
    int countErrors = 0;
    foreach(const ReplaceFile& file, files) {
        QString path = file.path();
        QList<ReplaceItem> items = file.items();
        QStringList lines = FileIO::readLines(path);
        bool ok = true;
        foreach(const ReplaceItem& item, items) {
            if (lines.size() <= item.line()) {
                qDebug() << "lines.size() <= item.line()";
                ok = false;
                break;
            }
            if (lines[item.line()] != item.before()) {
                ok = false;
                qDebug() << "lines[item.line()] != item.before()";
                qDebug() << lines[item.line()] << item.before();
                break;
            }
            lines[item.line()] = item.after();
        }
        if (ok) {
            ok = FileIO::writeLines(file.path(), lines);
            //ok = true;
            if (ok) {
                countFiles++;
                countLines += items.size();
            } else {
                countErrors++;
            }
        } else {
            countErrors++;
        }
    }
    emit replaced(countFiles, countLines);
}

void Worker::onFinishSearch(int id) {
    //emit canReplace(id, mCache.isPreview(id));
    //mCache.finish(id);
}

void Worker::onCanReplace(int id) {
    //emit canReplace(id, mCache.isPreview(id));
}
