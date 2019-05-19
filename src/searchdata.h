#ifndef SEARCHDATA_H
#define SEARCHDATA_H
#include <QString>
#include <QStringList>

class SearchData {
public:
    SearchData();
    SearchData(const QStringList& files, int filesFiltered, int dirsFiltered);
    QStringList files() const;
    void setFiles(const QStringList& files);
    int filesComplete() const;
    void setFilesComplete(int filesComplete);
    int filesFiltered() const;
    void setFilesFiltered(int filesFiltered);
    int dirsFiltered() const;
    void setDirsFiltered(int dirsFiltered);
    int filesSize() const;
    QString file(int i) const;

protected:
    QStringList mFiles;
    int mFilesComplete;
    int mFilesFiltered;
    int mDirsFiltered;
};
#endif // SEARCHDATA_H
