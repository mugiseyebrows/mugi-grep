#ifndef WORKER2_H
#define WORKER2_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>

#include "regexp.h"
#include "regexppath.h"
#include <QTextCodec>
#include <QDateTime>

#include <QDirIterator>
#include "searchparams.h"

#include "searchcache.h"
#include "searchhits.h"


#include "replaceparams.h"

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
    
    void found(int,SearchHits);

    void count(int,int);

    void allFiles(QString, QStringList);

    void canReplace(int, bool);

    void replaced(int,int);

    //void previewed(int, SearchHits);

public slots:

    void onCanReplace(int);

    void onFinishSearch(int);
    void onCountMatchedFiles(QString path, RegExpPath filter, bool notBinary);
    void onGetAllFiles(QString path);
    //void onSearch(int action, int searchId, QString path, RegExpPath filter, bool notBinary, RegExp search, int linesBefore, int linesAfter, bool cacheFileList, QString relpacement);
    void onSearch(SearchParams);
    void onSearchMore(int);

    void onReplace(ReplaceParams params);
};

#endif // WORKER2_H
