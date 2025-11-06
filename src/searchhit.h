#ifndef SEARCHHIT_H
#define SEARCHHIT_H
#include "linecontext.h"
#include <QList>
#include <QMap>
#include <QMetaType>
#include <QSet>
#include <QString>

class SearchHit {

public:
    SearchHit();
    SearchHit(const QString& path, const QString& relativePath, const QList<int>& hits,
              const QList<QPair<int,int>>& hits2 = {});

    QSet<int> siblings(int before, int after) const;
    void clearCache();
    LineContextItem context(int line) const;
    void read(int before, int after);
    QString path() const;
    void setPath(const QString& value);
    QString relativePath() const;
    void setRelativePath(const QString& value);
    QList<int> hits() const;
    void setHits(const QList<int>& value);
    QMap<int, QString> cache() const;
    void setCache(const QMap<int, QString>& value);
    LineContext context() const;
    void setContext(const LineContext& value);

    bool isEmpty() const;

    bool operator ==(const SearchHit& other) const;

    QList<QPair<int,int>> hits2() const;

    QSet<int> matched() const;
protected:
    QString mPath;
    QString mRelativePath;
    QList<int> mHits;
    QList<QPair<int,int> > mHits2;

};

Q_DECLARE_METATYPE(SearchHit)

#endif // SEARCHHIT_H
