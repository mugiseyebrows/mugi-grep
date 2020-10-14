#ifndef FILECOUNTMANAGER_H
#define FILECOUNTMANAGER_H

#include <QObject>

#include "regexppath.h"

#include "countfilesparams.h"

class CountFilesManager : public QObject
{
    Q_OBJECT
public:
    explicit CountFilesManager(QObject *parent = nullptr);
    bool contains(const QString& path, const RegExpPath& filter);
    QPair<int,int> count(const QString& path, const RegExpPath& filter);

protected:
    QList<CountFilesParams> mData;

signals:
    void countFiles(CountFilesParams);
    void filesCounted();

public slots:
    void onFilesCounted(CountFilesParams);
};

#endif // FILECOUNTMANAGER_H
