#include "searchnamehits.h"
#include <QString>

SearchNameHits::SearchNameHits() {
}
SearchNameHits::SearchNameHits(const RegExpPair& pattern) : mPattern(pattern) {
}
RegExpPair SearchNameHits::pattern() const {
    return mPattern;
}
void SearchNameHits::setPattern(const RegExpPair& value) {
    mPattern = value;
}
QStringList SearchNameHits::hits() const {
    return mHits;
}
void SearchNameHits::setHits(const QStringList& value) {
    mHits = value;
}
void SearchNameHits::append(const SearchNameHits& hits) {
    mPattern = hits.pattern();
    mHits.append(hits.hits());
}
void SearchNameHits::append(const QString& path) {
    mHits.append(path);
}
void SearchNameHits::clear() {
    mHits.clear();
}
