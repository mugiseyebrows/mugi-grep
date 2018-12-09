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

public slots:

    void onSearch(int searchId, QString path, RegExpPath filter, bool notBinary, RegExp search, int linesBofore, int linesAfter);
    void onSearchMore(int);
    void onFinishSearch(int);

    //void onCalcSize();

};

#endif // WORKER2_H
