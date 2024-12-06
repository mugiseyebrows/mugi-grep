#ifndef TOSET_H
#define TOSET_H

#include <QSet>

template <typename T>
QSet<T> toSet(const QList<T>& values) {
    return QSet<T>(values.begin(), values.end());
}

#endif // TOSET_H
