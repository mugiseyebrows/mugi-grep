#include "searchcache.h"

#include <QSet>
#include <QTextCodec>
#include <QDebug>

#include "html.h"
#include "filereader.h"
#include "utils/isbinext.h"
#include "utils/lit.h"
#include "worker.h"

namespace {

QStringList toStringList(const QVariantList& vs) {
    QStringList res;
    foreach(const QVariant& v, vs) {
        res << v.toString();
    }
    return res;
}

}

QStringList searchLines(const QStringList& mLines, const QString& mPath, const QString& mRelativePath,
                        const RegExp& exp, int linesBefore, int linesAfter) {

    QStringList res;

    QSet<int> matched;
    QSet<int> siblings;

    // pass 1
    for(int i=0;i<mLines.size();i++) {
        if (exp.match(mLines[i])) {
            matched << i;
        }
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

int backreferenceLength(const QString& replacement, int pos) {
    if (replacement[pos] == '\\' && pos + 1 < replacement.size() && replacement[pos+1].isDigit()) {
        int len = 1;
        while(++pos < replacement.size()) {
            if (!replacement[pos].isDigit()) {
                break;
            }
            len++;
        }
        return len;
    }
    return -1;
}

QVariantList tokenize(const QString& replacement) {
    QVariantList result;
    int prev = 0;
    int i = 0;
    while (i < replacement.size()) {
        int len = backreferenceLength(replacement, i);
        if (len > 0) {
            if (i - prev > 0) {
                QString part = replacement.mid(prev,i - prev);
                result.append(part);
            }
            QString ref = replacement.mid(i+1,len-1);
            result.append(ref.toInt());
            prev = i + len;
            i += len;
        } else {
            i++;
        }
    }
    if (prev < replacement.size()) {
        QString part = replacement.mid(prev);
        result.append(part);
    }
    return result;
}

void compare(const QVariantList& e, const QVariantList& a) {
    if (e == a) {
        return;
    }
    QString e_ = "{" + toStringList(e).join("|") + "}";
    QString a_ = "{" + toStringList(a).join("|") + "}";
    qDebug() << "not equal, expected: " << e_.toStdString().c_str() << ", actual " << a_.toStdString().c_str();
}


QString withBackreferences(const QRegularExpressionMatch& m, const QVariantList& replacement) {
    QStringList result;
    foreach(const QVariant& v, replacement) {
        if (v.type() == QVariant::Int) {
            result.append(m.captured(v.toInt()));
        } else if (v.type() == QVariant::String) {
            result.append(v.toString());
        } else {
            qDebug() << "withBackreferences error:" << v;
        }
    }
    return result.join("");
}

QStringList replacePreview(const QStringList& mLines, const QString& mPath, const QString& mRelativePath,
                           const RegExp& exp, const QString& replacement, QList<Replacement>& replacements) {

    QStringList res;

    QSet<int> matched;
    QSet<int> siblings;

    for(int i=0;i<mLines.size();i++) {
        if (exp.match(mLines[i])) {
            matched << i;
        }
    }

    if (matched.isEmpty()) {
        return res;
    }

    QRegularExpression rx = exp.includeExp();
    QVariantList replacement_ = tokenize(replacement);
    QString href = "file:///" + QDir::toNativeSeparators(mPath);
    res << Html::anchor(mRelativePath, href, "violet");
    foreach (int lineNum, matched) {
        QString line = mLines[lineNum];
        QRegularExpressionMatch m = rx.match(line);

        QStringList oldLine;
        oldLine << line.mid(0,m.capturedStart())
                << line.mid(m.capturedStart(),m.capturedLength())
                << line.mid(m.capturedEnd()+1);

        QStringList newLine;
        newLine << oldLine[0]
                << withBackreferences(m,replacement_)
                << oldLine[2];

        QStringList cols;
        cols << Html::span("- ","red")
             << Html::span(oldLine[0],"red")
             << Html::span(oldLine[1],"red",true)
             << Html::span(oldLine[2],"red");
        res << cols.join("");
        cols.clear();

        cols << Html::span("+ ","green")
             << Html::span(newLine[0],"green")
             << Html::span(newLine[1],"green",true)
             << Html::span(newLine[2],"green");
        res << cols.join("");
        cols.clear();
    }
    return res;
}

QStringList searchBinary(const QStringList& lines, const QString& path, const QString& relativePath,
                        const RegExp& exp) {
    QStringList res;
    foreach(const QString& line, lines) {
        if (exp.match(line)) {
            QString href = "file:///" + QDir::toNativeSeparators(path); // todo binary offset?
            QStringList cols;
            cols << Html::span("Binary file ", "black")
                 << Html::anchor(relativePath, href, "violet")
                 << Html::span(" matches", "black");
            res << cols.join("");
            break;
        }
    }
    return res;
}

QStringList searchLines(const QByteArray& bytes, const QString& path, const QString& relativePath,
                        const RegExp& exp, int linesBefore, int linesAfter, int* lineCount) {

    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QStringList lines = codec->toUnicode(bytes).split("\n");
    *lineCount = lines.size();
    return searchLines(lines,path,relativePath,exp,linesBefore,linesAfter);
}

QStringList replacePreview(const QByteArray& bytes, const QString& path, const QString& relativePath,
                        const RegExp& exp, int* lineCount, const QString& replacement, QList<Replacement>& replacements) {

    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QStringList lines = codec->toUnicode(bytes).split("\n");
    *lineCount = lines.size();
    return replacePreview(lines,path,relativePath,exp,replacement,replacements);
}

QStringList searchBinary(const QByteArray& bytes, const QString& path, const QString& relativePath,
                         const RegExp& exp) {

    QTextCodec* codec = QTextCodec::codecForName("UTF-8"); // todo guess or chose encoding
    QStringList lines = codec->toUnicode(bytes).split("\n");
    return searchBinary(lines,path,relativePath,exp);
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

    int filesFiltered_ = 0;
    int dirsFiltered_ = 0;

    QStringList files;

    foreach(const QString& path, allFiles) {
        if (path.contains("/.git/")) { // todo skip settings
            filesFiltered_++;
            continue;
        }
        if (notBinary && isBinExt(path)) {
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

void SearchCache::add(int action, int searchId, QString path, RegExpPath filter,
                      bool notBinary, RegExp search,
                      int linesBefore, int linesAfter, bool cacheFileList, const QString& replacement) {

    QMutexLocker locked(&mMutex);

    QStringList allFiles = getAllFiles(path,cacheFileList);
    int filesFiltered;
    int dirsFiltered;
    QStringList files = filterFiles(allFiles,filter,notBinary,&filesFiltered,&dirsFiltered);

    SearchData data = SearchData(action, path, filter, notBinary, search, files,
                                 linesBefore, linesAfter,
                                 filesFiltered, dirsFiltered, replacement);

    mSearchData.insert(searchId,data);
    mReplacements.insert(searchId,QList<Replacement>());
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

    SearchData& searchData = mSearchData[searchId];
    QList<Replacement>& replacements = mReplacements[searchId];

    //int lim = qMin(sd.complete + 100, sd.files.size());
    QStringList res;

    int lineCount = 0;
    int fileCount = 0;

    for (int i=searchData.complete;i<searchData.files.size();i++) {
        QString path = searchData.files[i];

        bool binary;
        bool readOk;
        bool tooBig;

        //QByteArray fileData = FileCache::instance()->file(path,sd.notBinary,&binary,&readOk);
        QByteArray fileData = FileReader::read(path,searchData.notBinary,&binary,&readOk,&tooBig);
        QString relPath_ = relPath(path,searchData.path);

        bool ok = true;

        if (!readOk) {
            qDebug() << "!readOk" << path;
            ok = false;
        }

        if (tooBig) { // todo implement line by line search in big files
            ok = false;
        }

        int fileLineCount = 0;

        if (ok) {
            if (searchData.action == Worker::Search) {
                if (binary) {
                    res << searchBinary(fileData, path, relPath_, searchData.search);
                } else {
                    res << searchLines(fileData, path, relPath_, searchData.search, searchData.linesBefore, searchData.linesAfter, &fileLineCount);
                }
            } else if (searchData.action == Worker::Preview) {
                res << replacePreview(fileData, path, relPath_, searchData.search, &fileLineCount, searchData.replacement, replacements);
            }
        }

        lineCount += fileLineCount;
        fileCount += 1;
        searchData.complete = i + 1;

        if (res.size() > 100 || lineCount > 4000 || fileCount > 50) {
            data = res.join("<br/>");
            *complete = searchData.complete;
            *total = searchData.files.size();
            *filtered = searchData.filesFiltered;
            file = relPath_;
            return;
        }
    }
    data = res.join("<br/>");
    *complete = searchData.complete;
    *total = searchData.files.size();
    *filtered = searchData.filesFiltered;
    file = QString();
}


void SearchCache::testTokenize() {

    qDebug() << "testTokenize() started";

    QString t;
    QVariantList a,e;

    t = "foo\\1bar\\2baz";
    e = Lit::vl("foo",1,"bar",2,"baz");
    a = tokenize(t);
    compare(e,a);

    t = "foo\\1bar\\2baz\\3";
    e = Lit::vl("foo",1,"bar",2,"baz",3);
    a = tokenize(t);
    compare(e,a);

    t = "foo\\1bar\\2baz\\3a";
    e = Lit::vl("foo",1,"bar",2,"baz",3,"a");
    a = tokenize(t);
    compare(e,a);

    t = "\\1bar\\2baz\\3";
    e = Lit::vl(1,"bar",2,"baz",3);
    a = tokenize(t);
    compare(e,a);

    t = "\\1bar\\2\\3baz\\52";
    e = Lit::vl(1,"bar",2,3,"baz",52);
    a = tokenize(t);
    compare(e,a);

    qDebug() << "testTokenize() finished";
}

