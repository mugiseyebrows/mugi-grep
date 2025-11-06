#include "searchhitwithcontext.h"

#include "toset.h"
#include <QFile>
#include <QDebug>

SearchHitWithContext::SearchHitWithContext()
{

}

QMap<int, QString> SearchHitWithContext::cache() const {
    return mCache;
}
void SearchHitWithContext::setCache(const QMap<int, QString>& value) {
    mCache = value;
}
LineContext SearchHitWithContext::context() const {
    return mContext;
}
void SearchHitWithContext::setContext(const LineContext& value) {
    mContext = value;
}

QList<int> SearchHitWithContext::matchedLines() const {
    if (mHits2.isEmpty()) {
        return mHits;
    }
    QSet<int> res;
    for(const QPair<int,int>& p: mHits2) {
        for(int i=p.first;i<=p.second;i++) {
            res.insert(i);
        }
    }
    return toList(res);
}
QSet<int> SearchHitWithContext::siblings(int linesBefore, int linesAfter) const {

    QSet<int> result;

    if (mHits2.isEmpty()) {
        for(int line: mHits) {
            int begin = std::max(0, line - linesBefore);
            int end = mLineCount > -1 ? std::min(mLineCount - 1, line + linesAfter) : line + linesAfter;
            for (int i = begin; i <= end; i++) {
                if (i != line) {
                    result.insert(i);
                }
            }
        }
        return result;
    }

    // mHits2
    for(int i=0;i<mHits2.size();i++) {
        QPair<int, int> rng = mHits2[i];
        int begin = std::max(0, rng.first - linesBefore);
        int end = mLineCount > -1 ? std::min(mLineCount - 1, rng.second + linesAfter) : rng.second + linesAfter;
        for (int i = begin; i <= end; i++) {
            if (i < rng.first || i > rng.second) {
                result.insert(i);
            }
        }
    }
    return result;
}
void SearchHitWithContext::clearCache() {
    mCache = QMap<int, QString>();
}
LineContextItem SearchHitWithContext::context(int line) const {
    return mContext.context(line);
}

SearchHitWithContext::SearchHitWithContext(const SearchHit &hit, int linesBefore, int linesAfter) : SearchHit(hit) {
    read(linesBefore, linesAfter);
}

void SearchHitWithContext::read(int linesBefore, int linesAfter) {

    QSet<int> keys = toSet(mCache.keys());

    QSet<int> matched = this->matched();
    QSet<int> siblings = this->siblings(linesBefore, linesAfter);

    if (keys.contains(matched) && keys.contains(siblings)) {
        return;
    }
    //qDebug() << "read" << mPath;
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
        if (matched.contains(i) || siblings.contains(i)) {
            mCache[i] = line;
        }
        i++;
    }
    mLineCount = i;
    file.close();
    mContext.init(mPath);
}
