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

#if 0

class ReplaceData : QList<Replacement> {
public:
    ReplaceData();
};

class ReplaceCache {
public:
    void add(int id, const ReplaceData& data) {
        mData[id] = data;
    }
    ReplaceData data(int id) {
        return mData[id];
    }
protected:
    QMap<int,ReplaceData> mData;
};

#endif

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

public slots:

    void onReplace(int searchId);
    void onSearchMore(int);
    void onFinishSearch(int);
    void onCountMatchedFiles(QString path, RegExpPath filter, bool notBinary);
    void onGetAllFiles(QString path);
    void onSearch(int action, int searchId, QString path, RegExpPath filter, bool notBinary, RegExp search, int linesBefore, int linesAfter, bool cacheFileList, QString relpacement);
};

#endif // WORKER2_H
