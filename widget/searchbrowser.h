#ifndef SEARCHBROWSER_H
#define SEARCHBROWSER_H
#include "regexp.h"
#include "regexppath.h"
#include <QDate>
#include <QDateTime>
#include <QTextBrowser>

class QDebug;
class SearchBrowser : public QTextBrowser {
    Q_OBJECT
public:
    SearchBrowser(QWidget* parent = 0);
    ~SearchBrowser();
    int searchId() const;
    void setSearchId(int searchId);
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
    bool notBinary() const;
    void setNotBinary(bool notBinary);
    QDateTime changed() const;
    void setChanged(const QDateTime& changed);
    bool isExecuted() const;
    void copy(SearchBrowser* dest);

protected:
    int mSearchId;
    RegExp mExp;
    RegExpPath mFilter;
    int mLinesBefore;
    int mLinesAfter;
    bool mCacheFileList;
    bool mNotBinary;
    QDateTime mChanged;
};
#endif // SEARCHBROWSER_H
