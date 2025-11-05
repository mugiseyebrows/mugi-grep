#ifndef SEARCHSTATE_H
#define SEARCHSTATE_H

#include "regexppair.h"
#include "regexppath.h"

#include <QDirIterator>
#include <QMutex>
#include <QDebug>
#include "searchparams.h"
#include "searchdata.h"
#include "replacement.h"
#include "searchhits.h"
#include "searchnamehits.h"

QStringList searchLines(const QStringList &mLines, const QString& mPath, const QString& mRelativePath,
                        const SearchParams &params);

QStringList searchLines(const QByteArray& mLines, const QString& mPath, const QString& mRelativePath,
                        const SearchParams &params, int *lineCount);

QStringList fileLines(const QString& path, bool skipBinary, bool* binary = nullptr);


class SearchState {
public:

    SearchState();

    void begin(SearchParams params);

    //void replace(int searchId, int* filesChanged, int* linesChanged, QStringList& notChanged);

    void end(int searchId);

    QPair<SearchHits, SearchNameHits> search(int searchId);

    //QMap<QString,int> fileSize() const;

    QPair<int, int> countMatchedFiles(QString path, RegExpPath filter);

    QStringList getListing(QString path, bool cacheFileList);
    static QStringList filterFiles(const QStringList &allFiles, RegExpPath filter, int *filesFiltered, int *dirsFiltered);
    static void testTokenize();

    //bool isPreview(int searchId);

    bool isFinished(int searchId);
    bool isPreviewFinished(int searchId);

    QString getCachedListingPath(const QString &path);
protected:

    QMutex mMutex;
    QMap<int,SearchParams> mSearchParams;

    QMap<int,SearchData> mSearchData;

    QMap<QString, QStringList> mFileList;



};

#endif // SEARCHSTATE_H
