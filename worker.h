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

signals:
    
    void found(int,QString,int,int,int,QString);

    void count(int,int);

    void allFiles(QString, QStringList);

    void canReplace(int, bool);

public slots:

    void onCanReplace(int);
    void onReplace(int searchId);
    void onSearchMore(int);
    void onFinishSearch(int);
    void onCountMatchedFiles(QString path, RegExpPath filter, bool notBinary);
    void onGetAllFiles(QString path);
    //void onSearch(int action, int searchId, QString path, RegExpPath filter, bool notBinary, RegExp search, int linesBefore, int linesAfter, bool cacheFileList, QString relpacement);
    void onSearch(SearchParams);
};

#endif // WORKER2_H
