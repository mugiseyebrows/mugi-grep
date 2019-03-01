#ifndef SEARCHBROWSER_H
#define SEARCHBROWSER_H
#include "regexp.h"
#include "regexppath.h"
#include <QTextBrowser>

class QDebug;
class SearchBrowser : public QTextBrowser {
    Q_OBJECT
public:
    SearchBrowser(QWidget* parent = 0);
    ~SearchBrowser();
    RegExp exp() const;
    void setExp(const RegExp& exp);
    RegExpPath filter() const;
    void setFilter(const RegExpPath& filter);
    int linesBefore() const;
    void setLinesBefore(int linesBefore);
    int linesAfter() const;
    void setLinesAfter(int linesAfter);
    bool cacheFileList() const;
    void setCacheFileList(bool cacheFileList);
    int searchId() const;
    void setSearchId(int searchId);
    bool notBinary() const;
    void setNotBinary(bool notBinary);
    bool isExecuted() const;
    void copy(SearchBrowser* dest);

protected:
    RegExp mExp;
    RegExpPath mFilter;
    int mLinesBefore;
    int mLinesAfter;
    bool mCacheFileList;
    int mSearchId;
    bool mNotBinary;
};
#endif // SEARCHBROWSER_H
