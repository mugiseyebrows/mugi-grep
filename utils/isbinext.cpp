#include "isbinext.h"

#include <QStringList>

namespace {

QString ext(const QString& path) {
    if (path.indexOf(".")>-1)
        return path.split(".").last().toLower();
    return QString();
}

}

bool isBinExt(const QString &path)
{
    static QStringList binaryExts;
    if (binaryExts.isEmpty()) {
        binaryExts << "7z" << "a" << "avi" << "bin" << "bmp" << "cab" << "cdr" << "chw" << "db"
                   << "djvu" << "dll" << "doc" << "docx" << "dot" << "dwg" << "exe" << "flv"
                   << "gif" << "gz" << "ico" << "iso" << "jar" << "jpeg" << "jpg" << "mdb"
                   << "mp3" << "mp4" << "msi" << "o" << "obj" << "ods" << "odt" << "pdb" << "pdf"
                   << "png" << "ppt" << "pyc" << "rar" << "rtf" << "sqlite" << "swf" << "sys"
                   << "tgz" << "tiff" << "ttf" << "wav" << "wmv" << "xla" << "xls" << "xlsm"
                   << "xlsx" << "xlt" << "xmcd" << "zip";
    }
    return binaryExts.contains(ext(path));
}
