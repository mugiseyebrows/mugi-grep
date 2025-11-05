#include "mugigrep.h"

#include <QFile>
#include <QFileInfo>
#include "regexppair.h"
#include "searchhit.h"

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

// todo open error handling

// todo encodings settings, encodings for ext
SearchHit searchSingleline(const QString& path,
                                  const QString& relPath,
                                  const RegExpPair& pattern,
                                  bool searchBinary,
                                  qint64 memLim,
                                  qint64 bufSize,
                                  qint64* bytesRead) {

    QFileInfo info(path);
    *bytesRead = info.size();
    if (info.size() > memLim) {
        qDebug() << "read block by block, buf size" << bufSize;
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "cannot read" << path;
            return {};
        }
        bool binary;
        bool tested = false;
        int offset = 0;
        QList<int> matched;
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
            int pos = data.lastIndexOf('\n');
            QByteArray head = data.mid(0, pos);
            QString text;
            if (binary) {
                text = QString::fromLocal8Bit(head);
            } else {
                text = QString::fromUtf8(head);
            }
            QStringList lines = text.split("\n");
            QList<int> matched1 = getMatched(lines, pattern, offset);
            matched.append(matched1);
            offset += lines.size();
            data = data.mid(pos + 1);
        }
        if (matched.isEmpty()) {
            return {};
        }
        return SearchHit(path, relPath, matched);
    }

    qDebug() << "read whole file into memory";

    // file is relativly small, can be read in memory
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "cannot read" << path;
        return {};
    }
    QByteArray data = file.readAll();
    bool binary = data.indexOf(0) > -1;
    if (binary && !searchBinary) {
        qDebug() << "skip binary file" << path;
        return {};
    }
    QString text;
    if (binary) {
        text = QString::fromLocal8Bit(data);
    } else {
        text = QString::fromUtf8(data);
    }
    QStringList lines = text.split("\n");
    QList<int> matched = getMatched(lines, pattern);
    if (matched.isEmpty()) {
        return {};
    }
    return SearchHit(path, relPath, matched);
}

SearchHit searchMultiline(const QString& path,
                                 const QString& relPath,
                                 const RegExpPair& pattern,
                                 bool binary,
                                 qint64 memLim,
                                 qint64 bufSize,
                                 qint64* bytesRead) {

}

