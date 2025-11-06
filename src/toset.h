#ifndef TOSET_H
#define TOSET_H

#include <QSet>

template <typename T>
QSet<T> toSet(const QList<T>& values) {
    return QSet<T>(values.begin(), values.end());
}

template <typename T>
static QList<T> toList(const QSet<T>& vs)
{
    return QList<T> (vs.constBegin(), vs.constEnd());
}

#endif // TOSET_H
