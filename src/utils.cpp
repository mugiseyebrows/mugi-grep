#include "utils.h"

QString Utils::ext(const QString& path) {
    if (path.indexOf(".")>-1)
        return path.split(".").last().toLower();
    return QString();
}

bool Utils::isBinExt(const QString &path)
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

QString Utils::relPath(const QString& path, const QString& base) {
    if (path.startsWith(base)) {
        if ((path[base.size()] == QChar('\\') || path[base.size()] == QChar('/')) && path.size() > base.size())
            return path.mid(base.size()+1);
        else
            return path.mid(base.size());
    }
    return path;
}


QStringList Utils::toStringList(const QVariantList& vs) {
    QStringList res;
    foreach(const QVariant& v, vs) {
        res << v.toString();
    }
    return res;
}
