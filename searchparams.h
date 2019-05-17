#ifndef SEARCHPARAMS_H
#define SEARCHPARAMS_H
#include "regexp.h"
#include "regexppath.h"
#include <QMetaType>
#include <QString>
#include <QStringList>

class SearchParams {
public:
    SearchParams();
    SearchParams(int action, int id, const QString& path, const RegExpPath& filter, bool skipBinary,
                 const RegExp& search, int linesBefore, int linesAfter, bool cacheFileList,
                 const QString& replace);
    int action() const;
    void setAction(int action);
    int id() const;
    void setId(int id);
    QString path() const;
    void setPath(const QString& path);
    RegExpPath filter() const;
    void setFilter(const RegExpPath& filter);
    bool skipBinary() const;
    void setSkipBinary(bool skipBinary);
    RegExp search() const;
    void setSearch(const RegExp& search);
    int linesBefore() const;
    void setLinesBefore(int linesBefore);
    int linesAfter() const;
    void setLinesAfter(int linesAfter);
    bool cacheFileList() const;
    void setCacheFileList(bool cacheFileList);
    QString replace() const;
    void setReplace(const QString& replace);
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
    int mAction;
    int mId;
    QString mPath;
    RegExpPath mFilter;
    bool mSkipBinary;
    RegExp mSearch;
    int mLinesBefore;
    int mLinesAfter;
    bool mCacheFileList;
    QString mReplace;
    QStringList mFiles;
    int mFilesComplete;
    int mFilesFiltered;
    int mDirsFiltered;
};
Q_DECLARE_METATYPE(SearchParams)

#endif // SEARCHPARAMS_H
