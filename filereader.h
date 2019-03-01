#ifndef FILEREADER_H
#define FILEREADER_H

#include <QByteArray>

class FileReader
{
public:
    FileReader();

    static QByteArray read(const QString& path, bool skipBinary, bool* binary, bool* readOk, bool *tooBig);

};

#endif // FILEREADER_H
