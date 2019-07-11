#ifndef SEARCHPARAMS_H
#define SEARCHPARAMS_H
#include "regexp.h"
#include "regexppath.h"
#include <QMetaType>
#include <QString>

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
};
Q_DECLARE_METATYPE(SearchParams)

#endif // SEARCHPARAMS_H
