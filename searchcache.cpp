#include "searchcache.h"

#include <QSet>
#include <QTextCodec>



QStringList searchLines(const QStringList& mLines, const QString& mPath, const QString& mRelativePath,
                        const RegExp& exp, int linesBefore, int linesAfter) {

    QStringList res;

    QSet<int> matched;
    QSet<int> siblings;

    // pass 1
    for(int i=0;i<mLines.size();i++) {
        if (exp.match(mLines[i]))
            matched << i;
    }

    int match;
    foreach(match,matched) {
        for (int i=0;i<linesBefore;i++) {
            siblings << match - i - 1;
        }
    }
    foreach(match,matched) {
        for (int i=0;i<linesAfter;i++) {
            siblings << match + i + 1;
        }
    }

    // pass 2
    for(int i=0;i<mLines.length();i++) {

        if (matched.contains(i)) {

            QString line = mLines[i];
            /*int p = line.indexOf(include);
            int matchedLength = include.matchedLength();*/
            int p;
            int matchedLength;
            exp.match(mLines[i],&p,&matchedLength);
            QString href = "file:///" + QDir::toNativeSeparators(mPath) + "?line=" + QString::number(i+1);
            QStringList cols;
            cols << anchor(mRelativePath, href, "violet") //span(mRelativePath, "violet")
                 << span(":", "blue")
                 << span(QString::number(i+1), "green")
                 << span(":", "blue")
                 << span(line.mid(0,p), "black")
                 << span(line.mid(p, matchedLength), "red")
                 << span(line.mid(p + matchedLength), "black");
            res << cols.join("");

        } else if (siblings.contains(i)) {

            QString line = mLines[i];
            QString href = "file:///" + QDir::toNativeSeparators(mPath) + "?line=" + QString::number(i+1);
            QStringList cols;
            cols << anchor(mRelativePath, href, "violet")
                 << span("-", "blue")
                 << span(QString::number(i+1), "green")
                 << span("-", "blue")
                 << span(line, "black");
            res << cols.join("");

        }
    }

    return res;
}

QStringList searchLines(const QByteArray& mLines, const QString& mPath, const QString& mRelativePath,
                        const RegExp& exp, int linesBefore, int linesAfter, int* lineCount) {

    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QStringList lines = codec->toUnicode(mLines).split("\n");
    *lineCount = lines.size();
    return searchLines(lines,mPath,mRelativePath,exp,linesBefore,linesAfter);
}

#if 0
QStringList fileLines(const QString& path, bool skipBinary, bool* binary) {

    QFile file(path);
    file.open(QIODevice::ReadOnly);

    if (binary)
        *binary = false;

    if (skipBinary) {
        QByteArray dataSample = file.read(2 * 1024);
        if (dataSample.indexOf('\0') > -1) {
            qDebug() << path << " is binary";
            if (skipBinary) {
                file.close();
                if (binary)
                    *binary = true;
                return QStringList();
            }
        } else {
            qDebug() << path << " is not binary";
        }
        file.seek(0);
    }

    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForName("UTF-8")); /* @todo select textcodec */

    QStringList lines = stream.readAll().split(QRegExp("\\r?\\n"));
    file.close();
    return lines;
}
#endif

QString ext(const QString& path) {
    if (path.indexOf(".")>-1)
        return path.split(".").last().toLower();
    return QString();
}

QString span(const QString &text_, const QString &color)
{
    QString text = text_;
    text.replace("<","&lt;").replace(">","&gt;");
    return QString("<span style=\"color:%1\">%2</span>").arg(color).arg(text);
}

QString anchor(const QString &text, const QString& path, const QString& color)
{
    return QString("<a href=\"%1\" style=\"color:%2; text-decoration: none\" >%3</a>").arg(path).arg(color).arg(text);
}

QString relPath(const QString& path, const QString& base) {
    if (path.startsWith(base)) {
        if (path[base.size()] == QChar('\\') && path.size() > base.size())
            return path.mid(base.size()+1);
        else
            return path.mid(base.size());
    }
    return path;
}

SearchCache::SearchCache() {

}

void SearchCache::add(int searchId, QString path, RegExpPath filter, bool notBinary, RegExp search, int linesBefore, int linesAfter) {
    QMutexLocker locked(&mMutex);
    QStringList files;

    // todo skip directory entirely if filter matches

    QDirIterator it(path, QDir::Files, QDirIterator::Subdirectories);

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

    while (it.hasNext()) {
        QString path = it.next();
        if (path.contains("/.git/"))
            continue;
        if (notBinary && binaryExts.contains(ext(path)))
            continue;
        if (filter.match(path)) {
            files << path;
        }
    }
    mSearchData.insert(searchId,SearchData(path,filter,notBinary,search,files,linesBefore,linesAfter));
}

void SearchCache::finish(int searchId) {
    QMutexLocker locked(&mMutex);
    mSearchData.remove(searchId);
}

void SearchCache::search(int searchId, QString &data, int *complete, int *total, int *filtered, QString &file) {
    QMutexLocker locked(&mMutex);

    if (!mSearchData.contains(searchId)) {
        qDebug() << "!mSearchData.contains(searchId)";
        return;
    }

    SearchData& sd = mSearchData[searchId];
    //int lim = qMin(sd.complete + 100, sd.files.size());
    QStringList res;

    int lineCount = 0;
    int fileCount = 0;

    for (int i=sd.complete;i<sd.files.size();i++) {
        QString path = sd.files[i];
        /*if (!mCache.contains(path)) {
                mCache[path] = fileLines(path,sd.notBinary);
                mLastModified[path] = QFileInfo(path).lastModified();
                mFileSize[path] = QFileInfo(path).size();
            } else {
                QDateTime lastModified = QFileInfo(path).lastModified();
                if (lastModified > mLastModified[path]) {
                    mCache[path] = fileLines(path,sd.notBinary);
                    mLastModified[path] = lastModified;
                    mFileSize[path] = QFileInfo(path).size();
                    //qDebug() << "changed" << path;
                } else {
                    //qDebug() << "not changed" << path;
                }
            }*/

        bool binary;
        bool readOk;

        QByteArray fileData = FileCache::instance()->file(path,sd.notBinary,&binary,&readOk);

        QString relPath_ = relPath(path,sd.path);

        int fileLineCount = 0;

        res << searchLines(fileData, path, relPath_, sd.search, sd.linesBefore, sd.linesAfter, &fileLineCount);
        lineCount += fileLineCount;
        fileCount += 1;
        sd.complete = i + 1;

        if (res.size() > 100 || lineCount > 4000 || fileCount > 50) {
            data = res.join("<br/>");
            *complete = sd.complete;
            *total = sd.files.size();
            *filtered = 0;
            file = relPath_;
            return;
        }
    }
    data = res.join("<br/>");
    *complete = sd.complete;
    *total = sd.files.size();
    *filtered = 0;
    file = QString();
}

