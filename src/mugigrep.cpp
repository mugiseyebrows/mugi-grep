#include "mugigrep.h"

#include <QFile>
#include <QFileInfo>
#include "regexppair.h"
#include "searchhit.h"

#if 0
QList<int> getMatched(const QStringList& lines, const RegExpPair& exp, int offset)
{
    QList<int> matched;
    for(int i=0;i<lines.size();i++) {
        if (exp.match(lines[i])) {
            matched.append(i + offset);
        }
    }
    return matched;
}
#endif

// todo open error handling

// todo encodings settings, encodings for ext
SearchHit searchSingleline(const QString& path,
                           const QString& relPath,
                           const RegExpPair& pattern,
                           bool searchBinary,
                           qint64 bufSize,
                           qint64* bytesRead) {

    QFileInfo info(path);
    *bytesRead = info.size();

    //qDebug() << "read block by block, buf size" << bufSize;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "cannot read" << path;
        return {};
    }

    bool binary;
    bool tested = false;
    int offset = 0;
    QList<int> hits;
    QByteArray data;
    while (!file.atEnd()) {
        data.append(file.read(bufSize));
        if (!tested) {
            binary = data.indexOf(0) > -1;
            if (binary && !searchBinary) {
                qDebug() << "skip binary file" << path;
                *bytesRead = bufSize;
                return {};
            }
            tested = true;
        }
        QByteArray head;
        if (file.atEnd()) {
            head = data;
            data = {};
        } else {
            int pos = data.lastIndexOf('\n');
            if (pos < 0) {
                head = data;
                data = {};
            } else {
                head = data.mid(0, pos);
                data = data.mid(pos + 1);
            }
        }
        QString text;
        if (binary) {
            text = QString::fromLocal8Bit(head);
        } else {
            text = QString::fromUtf8(head);
        }
        QStringList lines = text.split('\n');
        //QList<int> matched1 = getMatched(lines, pattern, offset);

        for(int i=0;i<lines.size();i++) {
            if (pattern.match(lines[i])) {
                hits.append(i + offset);
            }
        }

        offset += lines.size();
    }
    if (hits.isEmpty()) {
        return {};
    }
    return SearchHit(path, relPath, hits);
}


SearchHit searchMultiline(const QString& path,
                          const QString& relPath,
                          const RegExpPair& pattern,
                          bool searchBinary,
                          qint64 bufSize,
                          qint64* bytesRead) {

    QFileInfo info(path);
    *bytesRead = info.size();

    //qDebug() << "read block by block, buf size" << bufSize;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "cannot read" << path;
        return {};
    }
    bool binary;
    bool tested = false;
    int offset = 0;
    QList<int> hits;
    QList<QPair<int,int>> hits2;
    QByteArray data;
    while (!file.atEnd()) {
        data.append(file.read(bufSize));
        if (!tested) {
            binary = data.indexOf(0) > -1;
            if (binary && !searchBinary) {
                qDebug() << "skip binary file" << path;
                *bytesRead = bufSize;
                return {};
            }
            tested = true;
        }
        QByteArray head;
        int shift = 0;
        if (file.atEnd()) {
            head = data;
            data = {};
        } else {
            int pos = data.lastIndexOf('\n');
            if (pos < 0) {
                head = data;
            } else {
                head = data.mid(0, pos);
            }
            int pos2 = data.indexOf('\n', bufSize / 2);
            if (pos2 < 0) {
                // fix me
            } else {
                shift = data.mid(0, pos2).count('\n') + 1;
                data = data.mid(pos2 + 1);
            }
        }
        QString text;
        if (binary) {
            text = QString::fromLocal8Bit(head);
        } else {
            text = QString::fromUtf8(head);
        }
        auto it = pattern.includeExp().globalMatch(text);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            QString captured = match.captured(0);
            if (!pattern.exclude().isEmpty()) {
                if (pattern.excludeExp().match(captured).hasMatch()) {
                    continue;
                }
            }
            int pos = match.capturedStart(0);
            int line1 = text.mid(0, pos).count('\n') + offset;
            int line2 = line1 + captured.count('\n');

            if (!hits.contains(line1)) {
                if (!hits.isEmpty() && hits[hits.size() - 1] > line1) {
                    continue;
                }
                hits.append(line1);
                hits2.append({line1, line2});
            }
        }
        offset += shift;
    }
    if (hits.isEmpty() && hits2.isEmpty()) {
        return {};
    }
    return SearchHit(path, relPath, hits, hits2);
}

