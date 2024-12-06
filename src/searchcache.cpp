#include "searchcache.h"

#include <QSet>
#include <QDebug>

#include "html.h"
#include "fileio.h"

#include "utils.h"

#include <QStandardPaths>
#include <QCryptographicHash>

#if 0
QStringList bytesToLines(const QByteArray& bytes) {
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QStringList lines = codec->toUnicode(bytes).split("\n");
    return lines;
}

QByteArray linesToBytes(const QStringList& lines) {
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    return codec->fromUnicode(lines.join("\n"));
}
#endif

QStringList fileNameLineNumber(bool showFileName, bool showLineNumber, const QString& relativePath, const QString& href, int lineNumber) {
    QStringList cols;
    if (showFileName) {
        cols << Html::anchor(relativePath, href, "violet")
             << Html::span(":", "blue");
    }
    if (showLineNumber) {
        cols << Html::span(QString::number(lineNumber), "green")
        << Html::span(":", "blue");
    }
    return cols;
}

QList<int> getMatched(const QStringList& lines, const RegExp& exp)
{
    QList<int> matched;
    for(int i=0;i<lines.size();i++) {
        if (exp.match(lines[i])) {
            matched << i;
        }
    }
    return matched;
}

QSet<int> getSiblings(const QList<int>& matched, int linesBefore, int linesAfter) {
    QSet<int> siblings;
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
    return siblings;
}


#if 0
QStringList searchBinary(const QStringList& lines, const QString& path, const QString& relativePath,
                        const SearchParams& params) {
    QStringList res;
    RegExp exp = params.search();
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
#endif


void searchLines(const QStringList& lines, const QString& path, const QString& relativePath,
                        const SearchParams& params, SearchHits& hits) {
    QStringList res;
    RegExp exp = params.pattern();
    QList<int> matched = getMatched(lines, exp);
    if (matched.isEmpty()) {
        return;
    }
    SearchHit hit(path, relativePath, matched);
    hits.append(hit);
}

void searchLines(const QByteArray& bytes, const QString& path, const QString& relativePath,
                        const SearchParams& params, SearchHits& hits, int* lineCount) {
    QString text = QString::fromUtf8(bytes);
    QStringList lines = text.split(QRegularExpression("\\r?\\n"));
    *lineCount = lines.size();
    return searchLines(lines, path, relativePath, params, hits);
}

#if 0
QStringList replacePreview(const QByteArray& bytes, const QString& path, const QString& relativePath,
                           const SearchParams& params, int* lineCount, QList<Replacement>& replacements) {

    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QStringList lines = codec->toUnicode(bytes).split("\n");
    *lineCount = lines.size();
    return replacePreview(lines, path, relativePath, params, replacements);
}


QStringList searchBinary(const QByteArray& bytes, const QString& path, const QString& relativePath,
                         const SearchParams& params) {

    QTextCodec* codec = QTextCodec::codecForName("UTF-8"); // todo guess or chose encoding
    QStringList lines = codec->toUnicode(bytes).split("\n");
    return searchBinary(lines,path,relativePath,params);
}
#endif

SearchCache::SearchCache() {

}

QPair<int,int> SearchCache::countMatchedFiles(QString path, RegExpPath filter) {

    QMutexLocker locked(&mMutex);
    //qDebug() << filter << notBinary;

    qDebug() << "SearchCache::countMatchedFiles" << path << filter;

    QStringList allFiles = getListing(path, true);
    int filesFiltered;
    int dirsFiltered;
    // todo optimize
    QStringList files = filterFiles(allFiles,filter,&filesFiltered,&dirsFiltered);
    return QPair<int,int>(files.size(),allFiles.size());
}

QString SearchCache::getCachedListingPath(const QString& path) {
    QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(path.toUtf8());
    QString name = QString("%1-%2.txt").arg(FileIO::nameFromPath(path)).arg(QString::fromLatin1(hash.result().toHex(0)));
    return QDir(appData).filePath(name);
}

QStringList SearchCache::getListing(QString path, bool cacheFileList) {
    // todo skip directory entirely if filter matches if not cacheFileList
    QString path_ = QDir(path).absolutePath();
    QStringList allFiles;
    if (cacheFileList && mFileList.contains(path_)) {
        allFiles = mFileList[path_];
    } else {

        QString cachedListingPath;

        if (cacheFileList) {
            cachedListingPath = getCachedListingPath(path_);
            if (QFile::exists(cachedListingPath)) {
                allFiles = FileIO::readLines(cachedListingPath);
                mFileList[path_] = allFiles;
                return allFiles;
            }
        }

        QDirIterator it(path, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString path = it.next();
            allFiles.append(path);
        }
        if (cacheFileList) {
            mFileList[path_] = allFiles;
        }

        if (cacheFileList) {
            FileIO::writeLines(cachedListingPath, allFiles);
        }
    }
    if (!cacheFileList) {
        mFileList.remove(path_);
    }
    return allFiles;
}

QStringList SearchCache::filterFiles(const QStringList& allFiles, RegExpPath filter, int* filesFiltered, int* dirsFiltered) {

    int filesFiltered_ = 0;
    int dirsFiltered_ = 0;

    QStringList files;

    bool notBinary = filter.notBinary();

    foreach(const QString& path, allFiles) {
        if (path.contains("/.git/")) { // todo skip settings
            filesFiltered_++;
            continue;
        }
        if (notBinary && Utils::isBinExt(path)) {
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


void SearchCache::add(SearchParams params) {

    QMutexLocker locked(&mMutex);

    QString path = params.path();
    bool cacheFileList = params.cacheFileList();
    RegExpPath filter = params.filter();

    int searchId = params.id();

    QStringList allFiles = getListing(path, cacheFileList);
    int filesFiltered;
    int dirsFiltered;
    QStringList files = filterFiles(allFiles, filter, &filesFiltered, &dirsFiltered);

    SearchData data(files, filesFiltered, dirsFiltered);

    mSearchParams.insert(searchId,params);
    mSearchData.insert(searchId,data);
    //mReplacements.insert(searchId,QList<Replacement>());
}

void SearchCache::finish(int searchId) {

    QMutexLocker locked(&mMutex);
    mSearchData.remove(searchId);
    mSearchParams.remove(searchId);
}

#if 0
bool SearchCache::isPreview(int searchId) {
    QMutexLocker locked(&mMutex);
    return mSearchParams.contains(searchId) && mSearchParams[searchId].action() == Worker::Preview && mReplacements.contains(searchId);
}
#endif

bool SearchCache::isFinished(int searchId) {
    QMutexLocker locked(&mMutex);
    if (!mSearchData.contains(searchId) || !mSearchParams.contains(searchId)) {
        return true;
    }
    const SearchData& data = mSearchData[searchId];
    return data.filesComplete() >= data.filesSize();
}

QPair<SearchHits,SearchNameHits> SearchCache::search(int searchId) {
    QMutexLocker locked(&mMutex);

    if (!mSearchParams.contains(searchId) || !mSearchData.contains(searchId)) {
        qDebug() << "!mSearchParams.contains(searchId) || !mSearchData.contains(searchId)";
        return QPair<SearchHits,SearchNameHits>();
    }

    SearchParams& params = mSearchParams[searchId];
    SearchData& data = mSearchData[searchId];
    //QList<Replacement>& replacements = mReplacements[searchId];

    bool notBinary = params.filter().notBinary();

    SearchHits hits(params.pattern());
    hits.setFiltered(data.filesFiltered());

    //int lim = qMin(sd.complete + 100, sd.files.size());

    int lineCount = 0;
    int fileCount = 0;

    SearchNameHits nameHits(params.pattern());

    for (int i=data.filesComplete();i<data.filesSize();i++) {
        QString path = data.file(i);

        QString name = FileIO::nameFromPath(path);

        if (params.pattern().match(name)) {
            nameHits.append(path);
        }

        bool binary;
        bool readOk;
        bool tooBig;

        //QByteArray fileData = FileCache::instance()->file(path,sd.notBinary,&binary,&readOk);
        QByteArray fileData = FileIO::read(path,notBinary,&binary,&readOk,&tooBig);
        QString relPath = Utils::relPath(path,params.path());

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
            if (binary) {
                //res << searchBinary(fileData, path, relPath, searchParams);
            } else {
                searchLines(fileData, path, relPath, params, hits, &fileLineCount);
                hits.setLast(relPath);
            }
        }

        lineCount += fileLineCount;
        fileCount += 1;
        data.setFilesComplete(i + 1);


        if (lineCount > 4000 || fileCount > 50) {

            /*complete = searchData.filesComplete();
            total = searchData.filesSize();
            filtered = searchData.filesFiltered();*/

            hits.setComplete(data.filesComplete());
            hits.setTotal(data.filesSize());
            return QPair<SearchHits,SearchNameHits>(hits, nameHits);
        }
    }

    hits.setComplete(data.filesComplete());
    hits.setTotal(data.filesSize());
    return QPair<SearchHits,SearchNameHits>(hits,nameHits);
}

#if 0
void SearchCache::replace(int searchId, int* filesChanged, int* linesChanged, QStringList& notChanged) {
    QMutexLocker locked(&mMutex);
    if (!mReplacements.contains(searchId)) {
        return;
    }

    SearchParams params = mSearchParams[searchId];

    *filesChanged = 0;
    *linesChanged = 0;

    QList<Replacement> replacements = mReplacements[searchId];
    foreach(const Replacement& replacement, replacements) {
        bool readOk, tooBig, binary;
        QString path = replacement.path();
        QStringList lines = bytesToLines(FileReader::read(path,true,&binary,&readOk,&tooBig));
        if (!readOk) {
            qDebug() << "cannot read" << path;
            continue;
        }
        if (tooBig) {
            qDebug() << "too big" << path;
            continue;
        }
        bool ok = true;
        QList<ReplacementLine> replLines = replacement.lines();
        foreach (const ReplacementLine& replLine, replLines) {
            int lineNum = replLine.line();
            if (lines[lineNum] == replLine.oldLine()) {
                lines[lineNum] = replLine.newLine();
            } else {
                qDebug() << lines[lineNum];
                qDebug() << replLine.oldLine();
                ok = false;
            }
        }
        if (ok) {
            *filesChanged += 1;
            *linesChanged += replLines.size();
        } else {
            notChanged << Utils::relPath(path,params.path());
        }

        if (!ok) {
            qDebug() << "file changed" << path;
            continue;
        }
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "canot open file" << path;
            continue;
        }
        file.write(linesToBytes(lines));
        file.close();
    }

    mReplacements.remove(searchId);
}
#endif

