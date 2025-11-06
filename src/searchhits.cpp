#include "searchhits.h"

SearchHits::SearchHits() : mTotal(-1), mComplete(-1), mFiltered(-1) {
}
SearchHits::SearchHits(const RegExpPair& pattern, const QList<SearchHit>& hits)
    : mPattern(pattern), mHits(hits), mTotal(-1), mComplete(-1), mFiltered(-1) {
}
RegExpPair SearchHits::pattern() const {
    return mPattern;
}
void SearchHits::setPattern(const RegExpPair& value) {
    mPattern = value;
}
QList<SearchHit> SearchHits::hits() const {
    return mHits;
}
void SearchHits::setHits(const QList<SearchHit>& value) {
    mHits = value;
}
int SearchHits::total() const {
    return mTotal;
}
void SearchHits::setTotal(int value) {
    mTotal = value;
}
int SearchHits::complete() const {
    return mComplete;
}
void SearchHits::setComplete(int value) {
    mComplete = value;
}
int SearchHits::filtered() const {
    return mFiltered;
}
void SearchHits::setFiltered(int value) {
    mFiltered = value;
}
QString SearchHits::last() const {
    return mLast;
}
void SearchHits::setLast(const QString& value) {
    mLast = value;
}
void SearchHits::append(const SearchHits& hits) {
    mPattern = hits.pattern();
    mHits.append(hits.hits());
}
void SearchHits::append(const SearchHit& hit) {
    mHits.append(hit);
}
int SearchHits::size() const {
    return mHits.size();
}
bool SearchHits::isEmpty() const {
    return mHits.isEmpty();
}
SearchHit SearchHits::hit(int index) const {
    return mHits[index];
}
SearchHits SearchHits::mid(int index) {
    return SearchHits(mPattern, mHits.mid(index));
}
#if 0

void SearchHits::clearCache() {
    for (int i = 0; i < mHits.size(); i++) {
        mHits[i].clearCache();
    }
}
#endif
void SearchHits::clear() {
    mHits.clear();
}
