#include "searchhit.h"
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include "toset.h"
#include <QTextStream>

SearchHit::SearchHit() : mLineCount(-1) {
}
SearchHit::SearchHit(const QString& path, const QString& relativePath, const QList<int>& hits,
                     const QMap<int, QString>& cache, const LineContext& context, int lineCount)
    : mPath(path), mRelativePath(relativePath), mHits(hits), mCache(cache), mContext(context),
      mLineCount(lineCount) {
}
QString SearchHit::path() const {
    return mPath;
}
void SearchHit::setPath(const QString& value) {
    mPath = value;
}
QString SearchHit::relativePath() const {
    return mRelativePath;
}
void SearchHit::setRelativePath(const QString& value) {
    mRelativePath = value;
}
QList<int> SearchHit::hits() const {
    return mHits;
}
void SearchHit::setHits(const QList<int>& value) {
    mHits = value;
}
QMap<int, QString> SearchHit::cache() const {
    return mCache;
}
void SearchHit::setCache(const QMap<int, QString>& value) {
    mCache = value;
}
LineContext SearchHit::context() const {
    return mContext;
}
void SearchHit::setContext(const LineContext& value) {
    mContext = value;
}
QSet<int> SearchHit::siblings(int before, int after) const {

    QSet<int> result;
    foreach (int line, mHits) {
        int begin = qMax(0, line - before);
        int end = mLineCount > -1 ? qMin(mLineCount - 1, line + after) : line + after;
        for (int i = begin; i <= end; i++) {
            if (i != line) {
                result.insert(i);
            }
        }
    }
    return result;
}
void SearchHit::clearCache() {
    mCache = QMap<int, QString>();
}
LineContextItem SearchHit::context(int line) const {
    return mContext.context(line);
}

void SearchHit::read(int before, int after) {


    QSet<int> keys = toSet(mCache.keys());

    QSet<int> hits = toSet(mHits);
    QSet<int> siblings_ = siblings(before, after);

    if (keys.contains(hits) && keys.contains(siblings_)) {
        return;
    }
    qDebug() << "read" << mPath;
    QFile file(mPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "cannot open" << mPath;
        return;
    }
    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    int i = 0;
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        if (hits.contains(i) || siblings_.contains(i)) {
            mCache[i] = line;
        }
        i++;
    }
    mLineCount = i;
    file.close();
    mContext.init(mPath);
}
