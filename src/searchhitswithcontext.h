#ifndef SEARCHHITSWITHCONTEXT_H
#define SEARCHHITSWITHCONTEXT_H

#include "searchhits.h"
#include "searchhitwithcontext.h"

class SearchHitsWithContext
{
public:
    SearchHitsWithContext();

    SearchHitsWithContext(const RegExpPair& pattern, const QList<SearchHitWithContext>& hits, int total, int complete, int filtered, const QString& last);

    void append(const SearchHits &hits, int linesBefore, int linesAfter);

    void read(int linesBefore, int linesAfter);

    SearchHitsWithContext mid(qsizetype pos, qsizetype len = -1) const;

    qsizetype size() const;

    SearchHitWithContext hit(int index) const;

    RegExpPair pattern() const;
    QList<SearchHitWithContext> hits() const;
    int total() const;
    int complete() const;
    int filtered() const;
    QString last() const;

    bool isEmpty() const;

    void clear();

    void clearCache();

protected:
    RegExpPair mPattern;
    QList<SearchHitWithContext> mHits;
    int mTotal;
    int mComplete;
    int mFiltered;
    QString mLast;
};

#endif // SEARCHHITSWITHCONTEXT_H
