#include "editordetector.h"

#include <QString>
#include <QLatin1Char>
#include <QFileInfo>

EditorDetector::EditorDetector()
{

}

QString executableName(const QString& name) {
#if defined(Q_OS_WIN32)
    return name + ".exe";
#else
    return name;
#endif
}

#include <QDir>

QString findInPath(const QString &fileName)
{
    const QString path = QString::fromLocal8Bit(qgetenv("PATH"));
#if defined(Q_OS_WIN32)
    QLatin1Char separator(';');
#else
    QLatin1Char separator(':');
#endif

    const QStringList paths = path.split(separator);
    for (const QString &path : paths) {
        QString path_ = QDir(path).filePath(fileName);
        QFileInfo fileInfo(path_);
        if (fileInfo.exists() && fileInfo.isFile() && fileInfo.isExecutable())
            return QDir::toNativeSeparators(path_);
    }

    return QString();
}



QString pathJoin(const QString& path1, const QString& path2) {
    return QDir(path1).filePath(path2);
}

bool exists(const QString& path) {
    QFileInfo info(path);
    return info.exists();
}

QString existing(const QStringList& bases, const QString& path) {
    for(const QString& base: bases) {
        if (base.isEmpty()) {
            continue;
        }
        QString path_ = pathJoin(base, path);
        if (exists(path_)) {
            return QDir::toNativeSeparators(path_);
        }
    }
    return QString();
}

QString findProgramFiles32() {
    QString path = QString::fromLocal8Bit(qgetenv("ProgramFiles(x86)"));
    if (!path.isEmpty() && exists(path)) {
        return path;
    }
    path = "C:\\Program Files (x86)";
    if (exists(path)) {
        return path;
    }
    return QString();
}

QString findProgramFiles64() {
    QString path = QString::fromLocal8Bit(qgetenv("ProgramFiles"));
    if (!path.isEmpty() && exists(path)) {
        return path;
    }
    path = "C:\\Program Files";
    if (exists(path)) {
        return path;
    }
    return QString();
}

QString findAppData() {
    QString path = QString::fromLocal8Bit(qgetenv("APPDATA"));
    if (!path.isEmpty() && exists(path)) {
        return path;
    }
    QString profile = QString::fromLocal8Bit(qgetenv("USERPROFILE"));
    if (profile.isEmpty()) {
        return QString();
    }
    path = pathJoin(profile,"AppData\\Roaming");
    if (exists(path)) {
        return path;
    }
    return QString();
}

QString findAppDataLocal() {
    QString path = findAppData();
    if (path.isEmpty()) {
        return QString();
    }
    QString local = QDir(QDir(path).dirName()).filePath("Local");
    if (exists(local)) {
        return local;
    }
    return QString();
}

namespace  {

QString quoted(const QString& path)
{
    if (path.indexOf(" ") > -1) {
        return "\"" + path + "\"";
    }
    return path;
}


QString joinSpace(const QStringList& args) {
    return args.join(" ");
}

}


QList<Editor> EditorDetector::detect()
{

    QString vsCode = findInPath(executableName("code"));
    QString qtCreator = findInPath(executableName("qtcreator"));
    QString geany = findInPath(executableName("geany"));

#if defined(Q_OS_WIN32)

    QString programFiles32 = findProgramFiles32();
    QString programFiles64 = findProgramFiles64();
    QString appDataLocal = findAppDataLocal();

    QString vsCode1 = existing({programFiles32, programFiles64, appDataLocal}, "Microsoft VS Code\\Code.exe");

    if (!vsCode1.isEmpty()) {
        vsCode = vsCode1;
    }

    QString geany1 = existing({programFiles32, programFiles64}, "Geany\\bin\\geany.exe");

    if (!geany1.isEmpty()) {
        geany = geany1;
    }

#endif

    QList<Editor> result;
    if (!vsCode.isEmpty()) {
        result << Editor("", joinSpace({quoted(vsCode),"-r","-g","%file%:%line%"}), Editor::Type::VsCode);
    }
    if (!qtCreator.isEmpty()) {
        result << Editor("", joinSpace({quoted(qtCreator),"-client","%file%:%line%"}), Editor::Type::QtCreator);
    }
    if (!geany.isEmpty()) {
        result << Editor("", joinSpace({quoted(geany),"-l","%line%","%file%"}), Editor::Type::Geany);
    }

    return result;
}
