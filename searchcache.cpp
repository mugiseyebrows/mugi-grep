#include "searchcache.h"

#include <QSet>
#include <QTextCodec>
#include <QDebug>

#include "html.h"

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
            cols << Html::anchor(mRelativePath, href, "violet") //span(mRelativePath, "violet")
                 << Html::span(":", "blue")
                 << Html::span(QString::number(i+1), "green")
                 << Html::span(":", "blue")
                 << Html::span(line.mid(0,p), "black")
                 << Html::span(line.mid(p, matchedLength), "red")
                 << Html::span(line.mid(p + matchedLength), "black");
            res << cols.join("");

        } else if (siblings.contains(i)) {

            QString line = mLines[i];
            QString href = "file:///" + QDir::toNativeSeparators(mPath) + "?line=" + QString::number(i+1);
            QStringList cols;
            cols << Html::anchor(mRelativePath, href, "violet")
                 << Html::span("-", "blue")
                 << Html::span(QString::number(i+1), "green")
                 << Html::span("-", "blue")
                 << Html::span(line, "black");
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

QString ext(const QString& path) {
    if (path.indexOf(".")>-1)
        return path.split(".").last().toLower();
    return QString();
}


// todo: remove
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

QPair<int,int> SearchCache::countMatchedFiles(QString path, RegExpPath filter, bool notBinary) {

    QMutexLocker locked(&mMutex);

    qDebug() << filter << notBinary;

    QStringList allFiles = getAllFiles(path, true);
    int filesFiltered;
    int dirsFiltered;
    // todo optimize
    QStringList files = filterFiles(allFiles,filter,notBinary,&filesFiltered,&dirsFiltered);
    return QPair<int,int>(files.size(),allFiles.size());
}

QStringList SearchCache::getAllFiles(QString path, bool cacheFileList) {
    // todo skip directory entirely if filter matches if not cacheFileList
    QString path_ = QDir(path).absolutePath();
    QStringList allFiles;
    if (cacheFileList && mFileList.contains(path_)) {
        allFiles = mFileList[path_];
    } else {
        QDirIterator it(path, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString path = it.next();
            allFiles.append(path);
        }
        if (cacheFileList) {
            mFileList[path_] = allFiles;
        }
    }
    if (!cacheFileList) {
        mFileList.remove(path_);
    }
    return allFiles;
}

QStringList SearchCache::filterFiles(const QStringList& allFiles, RegExpPath filter, bool notBinary, int* filesFiltered, int* dirsFiltered) {

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

    int filesFiltered_ = 0;
    int dirsFiltered_ = 0;

    QStringList files;

    foreach(const QString& path, allFiles) {
        if (path.contains("/.git/")) {
            filesFiltered_++;
            continue;
        }
        if (notBinary && binaryExts.contains(ext(path))) {
            filesFiltered_++;
            continue;
        }
        if (filter.match(path)) {
            files << path;
        } else {
            filesFiltered_++;
        }
    }

    *filesFiltered = filesFiltered_;
    *dirsFiltered = dirsFiltered_;

    return files;
}

void SearchCache::add(int searchId, QString path, RegExpPath filter,
                      bool notBinary, RegExp search,
                      int linesBefore, int linesAfter, bool cacheFileList) {

    QMutexLocker locked(&mMutex);


    QStringList allFiles = getAllFiles(path,cacheFileList);
    int filesFiltered;
    int dirsFiltered;
    QStringList files = filterFiles(allFiles,filter,notBinary,&filesFiltered,&dirsFiltered);

    SearchData data = SearchData(path,filter,notBinary,search,files,
                                 linesBefore,linesAfter,
                                 filesFiltered,dirsFiltered);

    mSearchData.insert(searchId,data);
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
            *filtered = sd.filesFiltered;
            file = relPath_;
            return;
        }
    }
    data = res.join("<br/>");
    *complete = sd.complete;
    *total = sd.files.size();
    *filtered = sd.filesFiltered;
    file = QString();
}

