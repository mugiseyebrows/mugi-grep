#ifndef SEARCHCACHE_H
#define SEARCHCACHE_H

#include "regexp.h"
#include "regexppath.h"
#include "struct/searchdata.h"

#include <QDirIterator>
#include <QMutex>
#include <QDebug>

class Replacement {
public:
    typedef QPair<int,QPair<QString,QString> > ReplaceLine;
    Replacement() {

    }
    Replacement(QString& path, QList<ReplaceLine>& replacements) : mPath(path), mReplacements(replacements) {

    }
protected:
    QString mPath;
    QList<ReplaceLine> mReplacements;
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

    void finish(int searchId);

    void search(int searchId, QString& data, int* complete, int* total, int* filtered, QString& file);

    //QMap<QString,int> fileSize() const;

    QPair<int, int> countMatchedFiles(QString path, RegExpPath filter, bool notBinary);
    QStringList getAllFiles(QString path, bool cacheFileList);
    static QStringList filterFiles(const QStringList &allFiles, RegExpPath filter, bool notBinary, int *filesFiltered, int *dirsFiltered);
    static void testTokenize();

protected:

    QMutex mMutex;
    QMap<int,SearchData> mSearchData;
    QMap<int,QList<Replacement>> mReplacements;
    QMap<QString, QStringList> mFileList;

};

#endif // SEARCHCACHE_H
