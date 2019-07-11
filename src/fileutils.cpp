#include "fileutils.h"

#include <QApplication>
#include <QFileInfo>
#include <QProcess>
#include <QDir>
#include <QMessageBox>
#include <QSettings>

// this is striped version of qt-creator code /src/plugins/coreplugin/fileutils.cpp

class Environment {
public:
    static Environment systemEnvironment();
    QString searchInPath(const QString& name) const;
protected:
    Environment();
    QStringList mPath;
};

Environment::Environment() {
    mPath = qEnvironmentVariable("PATH").split(";");
}

Environment Environment::systemEnvironment() {
    return Environment();
}

QString Environment::searchInPath(const QString& name) const {
    foreach (const QString& path, mPath) {
        QString filePath = QDir(path).filePath(name);
        if (QFile(filePath).exists()) {
            return filePath;
        }
    }
    return QString();
}

namespace HostOsInfo {

    bool isWindowsHost();
    bool isMacHost();
}

bool HostOsInfo::isWindowsHost() {
#ifdef Q_OS_WIN
    return true;
#else
    return false;
#endif
}

bool HostOsInfo::isMacHost() {
#ifdef Q_OS_MACOS
    return true;
#else
    return false;
#endif
}

class FileName : public QString {
public:
    FileName(const QString& name);
    static FileName fromString(const QString& name);
    QString fileName() const;
    QString toString() const;
};

FileName::FileName(const QString& name) : QString(name) {

}

FileName FileName::fromString(const QString& name) {
    return FileName(name);
}

QString FileName::fileName() const {
    return *this;
}

QString FileName::toString() const {
    return *this;
}


namespace UnixUtils {
QString defaultFileBrowser();
QString fileBrowser(const QSettings *settings);
QString substituteFileBrowserParameters(const QString &pre, const QString &file);
}

namespace ICore {
    QSettings* settings();
}

QSettings* ICore::settings() {
    return nullptr;
}

void showGraphicalShellError(QWidget *parent, const QString &app, const QString &error)
{
    const QString title = QApplication::translate("Core::Internal",
                                                  "Launching a file browser failed");
    const QString msg = QApplication::translate("Core::Internal",
                                                "Unable to start the file manager:\n\n%1\n\n").arg(app);
    QMessageBox mbox(QMessageBox::Warning, title, msg, QMessageBox::Close, parent);
    if (!error.isEmpty())
        mbox.setDetailedText(QApplication::translate("Core::Internal",
                                                     "\"%1\" returned the following error:\n\n%2").arg(app, error));
    mbox.exec();
}

QString UnixUtils::defaultFileBrowser()
{
    return QLatin1String("xdg-open %d");
}

QString UnixUtils::fileBrowser(const QSettings *settings)
{
    const QString dflt = defaultFileBrowser();
    if (!settings)
        return dflt;
    return settings->value(QLatin1String("General/FileBrowser"), dflt).toString();
}

QString UnixUtils::substituteFileBrowserParameters(const QString &pre, const QString &file)
{
    QString cmd;
    for (int i = 0; i < pre.size(); ++i) {
        QChar c = pre.at(i);
        if (c == QLatin1Char('%') && i < pre.size()-1) {
            c = pre.at(++i);
            QString s;
            if (c == QLatin1Char('d')) {
                s = QLatin1Char('"') + QFileInfo(file).path() + QLatin1Char('"');
            } else if (c == QLatin1Char('f')) {
                s = QLatin1Char('"') + file + QLatin1Char('"');
            } else if (c == QLatin1Char('n')) {
                s = QLatin1Char('"') + FileName::fromString(file).fileName() + QLatin1Char('"');
            } else if (c == QLatin1Char('%')) {
                s = c;
            } else {
                s = QLatin1Char('%');
                s += c;
            }
            cmd += s;
            continue;
        }
        cmd += c;
    }
    return cmd;
}

void FileUtils::showInGraphicalShell(QWidget *parent, const QString &pathIn)
{

    const QFileInfo fileInfo(pathIn);

    // Mac, Windows support folder or file.
    if (HostOsInfo::isWindowsHost()) {
        const FileName explorer = Environment::systemEnvironment().searchInPath(QLatin1String("explorer.exe"));
        if (explorer.isEmpty()) {
            QMessageBox::warning(parent,
                                 QApplication::translate("Core::Internal",
                                                         "Launching Windows Explorer Failed"),
                                 QApplication::translate("Core::Internal",
                                                         "Could not find explorer.exe in path to launch Windows Explorer."));
            return;
        }
        QStringList param;
        if (!fileInfo.isDir())
            param += QLatin1String("/select,");
        param += QDir::toNativeSeparators(fileInfo.canonicalFilePath());
        QProcess::startDetached(explorer.toString(), param);
    } else if (HostOsInfo::isMacHost()) {
        QStringList scriptArgs;
        scriptArgs << QLatin1String("-e")
                   << QString::fromLatin1("tell application \"Finder\" to reveal POSIX file \"%1\"")
                                         .arg(fileInfo.canonicalFilePath());
        QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
        scriptArgs.clear();
        scriptArgs << QLatin1String("-e")
                   << QLatin1String("tell application \"Finder\" to activate");
        QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
    } else {
        // we cannot select a file here, because no file browser really supports it...
        const QString folder = fileInfo.isDir() ? fileInfo.absoluteFilePath() : fileInfo.filePath();
        const QString app = UnixUtils::fileBrowser(ICore::settings());
        QProcess browserProc;
        const QString browserArgs = UnixUtils::substituteFileBrowserParameters(app, folder);
        bool success = browserProc.startDetached(browserArgs);
        const QString error = QString::fromLocal8Bit(browserProc.readAllStandardError());
        success = success && error.isEmpty();
        if (!success)
            showGraphicalShellError(parent, app, error);
    }
}
