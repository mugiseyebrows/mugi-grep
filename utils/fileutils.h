#ifndef FILEUTILS_H
#define FILEUTILS_H

class QWidget;
#include <QString>

// this is striped version of qt-creator code /src/plugins/coreplugin/fileutils.h

namespace FileUtils {
    void showInGraphicalShell(QWidget *parent, const QString &pathIn);
}


#endif // FILEUTILS_H
