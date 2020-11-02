#ifndef FILEIO_H
#define FILEIO_H

#include <QByteArray>
#include <QStringList>

class FileIO
{
public:
    FileIO();

    static QByteArray read(const QString& path, bool skipBinary, bool* binary, bool* readOk, bool *tooBig);

    static QString nameFromPath(const QString &path);
    static QString dirName(const QString &path);

    static QStringList readLines(const QString &path);
    static bool writeLines(const QString &path, const QStringList &lines);
    static bool writeLines(const QString &path, const QString &lines);
};

#endif // FILEIO_H
