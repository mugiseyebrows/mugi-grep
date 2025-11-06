#ifndef MUGIGREP_H
#define MUGIGREP_H

class RegExpPair;
class SearchHit;

#include <QString>

QList<int> getMatched(const QStringList& lines, const RegExpPair& exp, int offset = 0);

SearchHit searchSingleline(const QString& path,
                                  const QString& relPath,
                                  const RegExpPair& pattern,
                                  bool searchBinary,
                                  qint64 bufSize,
                                  qint64* bytesRead);


SearchHit searchMultiline(const QString& path,
                                 const QString& relPath,
                                 const RegExpPair& pattern,
                                 bool binary,
                                 qint64 bufSize,
                                 qint64* bytesRead);

#endif // MUGIGREP_H
