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

#include "struct/searchdata.h"

#include "filecache.h"
#include "searchcache.h"
class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    
protected:

    SearchCache mCache;

signals:
    
    void found(int,QString,int,int,int,QString);

    void count(int,int);

    void allFiles(QString, QStringList);

public slots:

    void onSearch(int searchId, QString path, RegExpPath filter, bool notBinary, RegExp search, int linesBofore, int linesAfter, bool cacheFileList);
    void onSearchMore(int);
    void onFinishSearch(int);
    void onCountMatchedFiles(QString path, RegExpPath filter, bool notBinary);
    void onGetAllFiles(QString path);
};

#endif // WORKER2_H
