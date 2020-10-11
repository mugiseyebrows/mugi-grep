#include "filereader.h"

#include <QIODevice>
#include <QFile>
#include "utils.h"

#define FILESIZE_TOOBIG (128*1024*1024)
#define DATASAMPLE_SIZE (2*1024)

FileReader::FileReader()
{

}

#include <QTextCodec>
#include <QFile>

QStringList FileReader::readLines(const QString& path) {

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return QStringList();
    }
    QByteArray bytes = file.readAll();
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QStringList lines = codec->toUnicode(bytes).split("\n");
    return lines;
}

QByteArray FileReader::read(const QString &path, bool skipBinary, bool *binary, bool *readOk, bool* tooBig)
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

    if (file.size() > DATASAMPLE_SIZE && skipBinary) {
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
