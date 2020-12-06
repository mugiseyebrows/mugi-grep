#include "fileio.h"

#include <QIODevice>
#include <QFile>
#include <QTextCodec>
#include <QFile>
#include "utils.h"
#include <QRegularExpression>

#define FILESIZE_TOOBIG (128*1024*1024)
#define DATASAMPLE_SIZE (2*1024)

FileIO::FileIO()
{

}

QStringList FileIO::readLines(const QString& path) {

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return QStringList();
    }
    QByteArray bytes = file.readAll();
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QStringList lines = codec->toUnicode(bytes).split(QRegularExpression("\\r?\\n"));
    return lines;

    /*bool binary, readOk, tooBig;
    QByteArray bytes = read(path, false, &binary, &readOk, &tooBig);
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QStringList lines = codec->toUnicode(bytes).split("\n");
    return lines;*/
}

bool FileIO::writeLines(const QString& path, const QString& line) {
    QStringList lines = {line};
    return writeLines(path, lines);
}

bool FileIO::writeLines(const QString& path, const QStringList& lines) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QByteArray data = codec->fromUnicode(lines.join("\n"));
    file.write(data);
    return true;
}


QString FileIO::nameFromPath(const QString& path) {
    int p = qMax(path.lastIndexOf('/'), path.lastIndexOf('\\'));
    if (p < 0) {
        return path;
    }
    return path.mid(p + 1);
}


QString FileIO::dirName(const QString& path) {
    int p = qMax(path.lastIndexOf('/'), path.lastIndexOf('\\'));
    if (p < 0) {
        return ".";
    }
    return path.mid(0,p);
}


QByteArray FileIO::read(const QString &path, bool skipBinary, bool *binary, bool *readOk, bool* tooBig)
{
    QFile file(path);
    *readOk = true;
    *binary = false;
    *tooBig = false;

    if (skipBinary && Utils::isBinExt(path)) {
        *binary = true;
        return QByteArray();
    }

    if (!file.open(QIODevice::ReadOnly)) {
        *readOk = false;
        return QByteArray();
    }

    if (file.size() > FILESIZE_TOOBIG) {
        *tooBig = true;
        return QByteArray();
    }

    if (skipBinary && file.size() > DATASAMPLE_SIZE) {
        QByteArray dataSample = file.read(DATASAMPLE_SIZE);
        if (dataSample.indexOf('\0') > -1) {
            *binary = true;
            return QByteArray();
        }
        file.seek(0);
    }

    QByteArray res = file.readAll();
    if (res.indexOf('\0') > -1) {
        *binary = true;
        if (skipBinary) {
            return QByteArray();
        }
    }

    return res;
}
