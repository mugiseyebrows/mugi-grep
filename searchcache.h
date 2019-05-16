#ifndef SEARCHCACHE_H
#define SEARCHCACHE_H

#include "regexp.h"
#include "regexppath.h"
#include "struct/searchdata.h"

#include <QDirIterator>
#include <QMutex>
#include <QDebug>

class ReplacementLine {
public:
    ReplacementLine() {

    }
    ReplacementLine(int line, const QString& oldLine, const QString& newLine) : mLine(line), mOldLine(oldLine), mNewLine(newLine) {

    }
    int line() const {
        return mLine;
    }
    QString oldLine() const {
        return mOldLine;
    }
    QString newLine() const {
        return mNewLine;
    }

protected:
    int mLine;
    QString mOldLine;
    QString mNewLine;
};


class Replacement {
public:
    Replacement() {

    }
    Replacement(const QString& path, const QList<ReplacementLine>& lines) : mPath(path), mLines(lines) {

    }
    QString path() const {
        return mPath;
    }
    QList<ReplacementLine> lines() const {
        return mLines;
    }
protected:
    QString mPath;
    QList<ReplacementLine> mLines;
};

QString ext(const QString& path);

QStringList searchLines(const QStringList &mLines, const QString& mPath, const QString& mRelativePath,
                        const RegExp& exp, int linesBefore, int linesAfter);

QStringList searchLines(const QByteArray& mLines, const QString& mPath, const QString& mRelativePath,
                        const RegExp& exp, int linesBefore, int linesAfter, int *lineCount);

QStringList fileLines(const QString& path, bool skipBinary, bool* binary = nullptr);

QString relPath(const QString& path, const QString& base);

class SearchCache {
public:
    SearchCache();

    void add(int action, int searchId, QString path, RegExpPath filter, bool notBinary, RegExp search,
            int linesBefore, int linesAfter, bool cacheFileList, const QString &replacement);

    void replace(int searchId);

    void finish(int searchId);

    void search(int searchId, QString& data, int* complete, int* total, int* filtered, QString& file);

    //QMap<QString,int> fileSize() const;

    QPair<int, int> countMatchedFiles(QString path, RegExpPath filter, bool notBinary);
    QStringList getAllFiles(QString path, bool cacheFileList);
    static QStringList filterFiles(const QStringList &allFiles, RegExpPath filter, bool notBinary, int *filesFiltered, int *dirsFiltered);
    static void testTokenize();

    bool isPreview(int searchId);
protected:

    QMutex mMutex;
    QMap<int,SearchData> mSearchData;
    QMap<int,QList<Replacement>> mReplacements;
    QMap<QString, QStringList> mFileList;

};

#endif // SEARCHCACHE_H
