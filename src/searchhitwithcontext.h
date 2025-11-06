#ifndef SEARCHHITWITHCONTEXT_H
#define SEARCHHITWITHCONTEXT_H

#include "searchhit.h"

class SearchHitWithContext : public SearchHit
{
public:
    SearchHitWithContext();
    SearchHitWithContext(const SearchHit& hit, int linesBefore = 0, int linesAfter = 0);
    void read(int before, int after);
    void clearCache();
    LineContextItem context(int line) const;
    QSet<int> siblings(int before, int after) const;
    QList<int> matchedLines() const;
    void setContext(const LineContext &value);
    LineContext context() const;
    void setCache(const QMap<int, QString> &value);
    QMap<int, QString> cache() const;
protected:
    QMap<int, QString> mCache;
    LineContext mContext;
    int mLineCount = -1;
};

#endif // SEARCHHITWITHCONTEXT_H
