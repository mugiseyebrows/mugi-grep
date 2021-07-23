#include "editordetector.h"

#include <QString>
#include <QLatin1Char>
#include <QFileInfo>
#include <QSettings>
#include <QDir>

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

QString pathJoin(const QStringList path) {
    QDir dir(path[0]);
    for(int i=1;i<path.size();i++) {
        dir = QDir(dir.filePath(path[i]));
    }
    return dir.path();
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
        QString path_ = pathJoin({base, path});
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

QString findAppDataRoaming() {
    QString path = QString::fromLocal8Bit(qgetenv("APPDATA"));
    if (!path.isEmpty() && exists(path)) {
        return path;
    }
    QString profile = QString::fromLocal8Bit(qgetenv("USERPROFILE"));
    if (profile.isEmpty()) {
        return QString();
    }
    path = pathJoin({profile,"AppData\\Roaming"});
    if (exists(path)) {
        return path;
    }
    return QString();
}

QString findAppDataLocal() {
    QString roaming = findAppDataRoaming();
    if (roaming.isEmpty()) {
        return QString();
    }

    QString local = pathJoin({roaming, "..", "Local"});
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
    QString appDataRoaming = findAppDataRoaming();

    QString vsCode1 = existing({programFiles32, programFiles64, appDataLocal}, "Microsoft VS Code\\Code.exe");

    if (!vsCode1.isEmpty()) {
        vsCode = vsCode1;
    }

    QString geany1 = existing({programFiles32, programFiles64}, "Geany\\bin\\geany.exe");

    if (!geany1.isEmpty()) {
        geany = geany1;
    }

    QString qtCreator1 = existing({"C:\\Qt\\Tools\\QtCreator\\bin"}, executableName("qtcreator"));
    if (!qtCreator1.isEmpty()) {
        qtCreator = qtCreator1;
    }

    QStringList regKeys = {
        "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
        "HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall"
    };

    QRegularExpression geanyRx("Geany [0-9.]+", QRegularExpression::CaseInsensitiveOption);

    for(const QString& regKey: regKeys) {
        QSettings settings(regKey, QSettings::NativeFormat);
        QStringList groups = settings.childGroups();
        for(const QString& group: groups) {
            settings.beginGroup(group);
            QString displayName = settings.value("DisplayName").toString();
            QString installLocation = settings.value("InstallLocation").toString();
            QString displayIcon = settings.value("DisplayIcon").toString();
            if (displayName == "Microsoft Visual Studio Code") {
                QString path = existing({installLocation},"Code.exe");
                if (!path.isEmpty()) {
                    vsCode = path;
                }
            }
            if (geanyRx.match(displayName).hasMatch()) {
                if (exists(displayIcon)) {
                    geany = displayIcon;
                }
            }
            settings.endGroup();
        }
    }

    QString startMenuQt = pathJoin({appDataRoaming, "Microsoft\\Windows\\Start Menu\\Programs\\Qt"});
    if (exists(startMenuQt)) {
        QStringList files = QDir(startMenuQt).entryList(QDir::Files);
        QRegularExpression rxQtCreator("Qt Creator",QRegularExpression::CaseInsensitiveOption);
        for(const QString& name: files) {
            if (rxQtCreator.match(name).hasMatch()) {
                QString path = QFile::symLinkTarget(pathJoin({startMenuQt, name}));
                if (exists(path)) {
                    qtCreator = path;
                }
            }
        }
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
