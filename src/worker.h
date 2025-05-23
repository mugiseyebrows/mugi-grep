#ifndef WORKER2_H
#define WORKER2_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>

#include "regexp.h"
#include "regexppath.h"
#include <QDateTime>

#include <QDirIterator>
#include "searchparams.h"

#include "searchcache.h"
#include "searchhits.h"
#include "countfilesparams.h"
#include "getlistingparams.h"
#include "searchnamehits.h"

#include "replaceparams.h"
#include "replacedparams.h"
#include "renameparams.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    enum Action {
        Search,
        Preview,
        Replace
    };

    explicit Worker(QObject *parent = nullptr);
    

protected:

    SearchCache mCache;

    void onMore(int id, int mode);

signals:
    
    void found(int,SearchHits,SearchNameHits);

    void count(int,int);

    void allFiles(QString, QStringList);

    void listing(QString, QStringList);

    void canReplace(int, bool);

    void replaced(ReplacedParams);

    void filesCounted(CountFilesParams);

    void renamed(int,int);

    //void previewed(int, SearchHits);

public slots:

    void onCanReplace(int);

    void onFinishSearch(int);
    void onCountMatchedFiles(QString path, RegExpPath filter);
    void onGetAllFiles(QString path);
    //void onSearch(int action, int searchId, QString path, RegExpPath filter, bool notBinary, RegExp search, int linesBefore, int linesAfter, bool cacheFileList, QString relpacement);
    void onSearch(SearchParams);
    void onSearchMore(int);

    void onReplace(ReplaceParams params);
    void onRename(RenameParams);
    void onCountFiles(CountFilesParams params);

    void onGetListing(GetListingParams params);
};

#endif // WORKER2_H
