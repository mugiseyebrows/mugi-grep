#ifndef SEARCHPARAMS_H
#define SEARCHPARAMS_H
#include "regexp.h"
#include "regexppath.h"
#include <QMetaType>
#include <QString>

class SearchParams {
public:
    SearchParams();
    SearchParams(int action, int id, const QString& path, bool cacheFileList,
                 const RegExpPath& filter, bool skipBinary, const RegExp& search, int linesBefore,
                 int linesAfter, bool showFileName, bool showLineNumber, bool onlyMatched,
                 const QString& replace, bool preserveCase);
    int action() const;
    int id() const;
    QString path() const;
    bool cacheFileList() const;
    RegExpPath filter() const;
    bool skipBinary() const;
    RegExp search() const;
    int linesBefore() const;
    int linesAfter() const;
    bool showFileName() const;
    bool showLineNumber() const;
    bool onlyMatched() const;
    QString replace() const;
    bool preserveCase() const;

protected:
    int mAction;
    int mId;
    QString mPath;
    bool mCacheFileList;
    RegExpPath mFilter;
    bool mSkipBinary;
    RegExp mSearch;
    int mLinesBefore;
    int mLinesAfter;
    bool mShowFileName;
    bool mShowLineNumber;
    bool mOnlyMatched;
    QString mReplace;
    bool mPreserveCase;
};
Q_DECLARE_METATYPE(SearchParams)

#endif // SEARCHPARAMS_H
