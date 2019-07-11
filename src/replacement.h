#ifndef REPLACEMENT_H
#define REPLACEMENT_H

#include <QList>
#include "replacementline.h"

class Replacement {
public:
    Replacement();
    Replacement(const QString& path, const QList<ReplacementLine>& lines);
    QString path() const;
    QList<ReplacementLine> lines() const;
protected:
    QString mPath;
    QList<ReplacementLine> mLines;
};

#endif // REPLACEMENT_H
