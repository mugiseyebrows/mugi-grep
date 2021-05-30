#ifndef PARSECPP_H
#define PARSECPP_H

#include "linecontext.h"

class MatchingMap {
public:

    MatchingMap(const QList<QPair<int,int>>& par_op,
                const QList<QPair<int,int>>& par_cl,
                const QList<QPair<int,int>>& br_op,
                const QList<QPair<int,int>>& br_cl);

    int matching(const QList<QPair<int,int>>& items, const QPair<int,int>& item, bool forward);

    int indexOf(const QList<QPair<int,int>>& items, int pos);

    int matching(int pos);

    QList<QPair<int,int>> par_op;
    QList<QPair<int,int>> par_cl;
    QList<QPair<int,int>> br_op;
    QList<QPair<int,int>> br_cl;

};

class ParseCpp
{
public:
    ParseCpp();

    static QList<LineContextItem> parse(const QString &path);

};

#endif // PARSECPP_H
