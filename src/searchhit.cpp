#include "searchhit.h"
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include "toset.h"
#include <QTextStream>

SearchHit::SearchHit() {
}
SearchHit::SearchHit(const QString& path, const QString& relativePath, const QList<int>& hits,
                     const QList<QPair<int,int> >& hits2)
    : mPath(path), mRelativePath(relativePath), mHits(hits), mHits2(hits2) {
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

bool SearchHit::isEmpty() const {
    return mHits.isEmpty() && mHits2.isEmpty();
}

bool SearchHit::operator ==(const SearchHit &other) const {
    return path() == other.path() && hits() == other.hits();
}

QList<QPair<int, int> > SearchHit::hits2() const {
    return mHits2;
}

QSet<int> SearchHit::matched() const {
    if (mHits2.isEmpty()) {
        return toSet(mHits);
    }
    QSet<int> res;
    for(int i=0;i<mHits2.size();i++) {
        QPair<int, int> rng = mHits2[i];
        for(int j=rng.first;j<=rng.second;j++) {
            res.insert(j);
        }
    }
    return res;
}



