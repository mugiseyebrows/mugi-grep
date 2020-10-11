#ifndef FILEREADER_H
#define FILEREADER_H

#include <QByteArray>
#include <QStringList>

class FileReader
{
public:
    FileReader();

    static QByteArray read(const QString& path, bool skipBinary, bool* binary, bool* readOk, bool *tooBig);

    static QStringList readLines(const QString &path);
};

#endif // FILEREADER_H
