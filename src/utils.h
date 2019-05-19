#ifndef UTLIS_H
#define UTLIS_H

#include <QStringList>
#include <QVariantList>

namespace Utils {

bool isBinExt(const QString& path);

QString ext(const QString& path);

QString relPath(const QString& path, const QString& base);

QStringList toStringList(const QVariantList& vs);

}
#endif // UTLIS_H
