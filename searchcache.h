#ifndef SEARCHCACHE_H
#define SEARCHCACHE_H

#include "regexp.h"
#include "regexppath.h"

#include <QDirIterator>
#include <QMutex>
#include <QDebug>
#include "searchparams.h"
#include "replacement.h"

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

    void add(SearchParams params);

    void replace(int searchId, int* filesChanged, int* linesChanged, QStringList& notChanged);

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
    QMap<int,SearchParams> mSearchData;
    QMap<int,QList<Replacement> > mReplacements;
    QMap<QString, QStringList> mFileList;

};

#endif // SEARCHCACHE_H
