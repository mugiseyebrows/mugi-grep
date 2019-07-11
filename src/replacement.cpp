#include "replacement.h"

Replacement::Replacement()
{

}

Replacement::Replacement(const QString &path, const QList<ReplacementLine> &lines) : mPath(path), mLines(lines) {

}

QString Replacement::path() const {
    return mPath;
}

QList<ReplacementLine> Replacement::lines() const {
    return mLines;
}
