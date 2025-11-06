#include "searchhitswithcontext.h"

SearchHitsWithContext::SearchHitsWithContext()
{

}

SearchHitsWithContext::SearchHitsWithContext(const RegExpPair &pattern, const QList<SearchHitWithContext> &hits, int total, int complete, int filtered, const QString &last) : mPattern(pattern), mHits(hits), mTotal(total), mComplete(complete), mFiltered(filtered), mLast(last) {

}

void SearchHitsWithContext::append(const SearchHits &hits, int linesBefore, int linesAfter) {
    for(int i=0;i<hits.size();i++) {
        SearchHitWithContext hit = SearchHitWithContext(hits.hit(i), linesBefore, linesAfter);
        mHits.append(hit);
    }
    mPattern = hits.pattern();
    mTotal = hits.total();
    mComplete = hits.complete();
    mFiltered = hits.filtered();
    mLast = hits.last();
}

void SearchHitsWithContext::read(int linesBefore, int linesAfter) {
    for (int i = 0; i < mHits.size(); i++) {
        mHits[i].read(linesBefore, linesAfter);
    }
}

SearchHitsWithContext SearchHitsWithContext::mid(qsizetype pos, qsizetype len) const {
    return SearchHitsWithContext(mPattern, mHits.mid(pos, len), mTotal, mComplete, mFiltered, mLast);
}

qsizetype SearchHitsWithContext::size() const {
    return mHits.size();
}

SearchHitWithContext SearchHitsWithContext::hit(int index) const {
    return mHits[index];
}

RegExpPair SearchHitsWithContext::pattern() const {
    return mPattern;
}

QList<SearchHitWithContext> SearchHitsWithContext::hits() const {
    return mHits;
}

int SearchHitsWithContext::total() const {
    return mTotal;
}

int SearchHitsWithContext::complete() const {
    return mComplete;
}

int SearchHitsWithContext::filtered() const {
    return mFiltered;
}

QString SearchHitsWithContext::last() const {
    return mLast;
}

bool SearchHitsWithContext::isEmpty() const {
    return mHits.isEmpty();
}

void SearchHitsWithContext::clear() {
    mHits.clear();
}

void SearchHitsWithContext::clearCache() {
    for(int i=0;i<mHits.size();i++) {
        mHits[i].clearCache();
    }
}
