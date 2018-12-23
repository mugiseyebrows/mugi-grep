#ifndef SEARCHBROWSER_H
#define SEARCHBROWSER_H

#include <QTextBrowser>

#include "regexp.h"
#include "regexppath.h"

class SearchBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    explicit SearchBrowser(RegExp exp, RegExpPath filter, int linesBefore, int linesAfter, bool cacheFileList, QWidget *parent = nullptr);
    
    ~SearchBrowser();

    RegExp exp() const;
    RegExpPath filter() const;
    int linesBefore() const;
    int linesAfter() const;
    bool cacheFileList() const;

protected:
    RegExp mExp;
    RegExpPath mFilter;
    int mLinesBefore;
    int mLinesAfter;
    bool mCacheFileList;

signals:
    
public slots:
    
};

#endif // SEARCHBROWSER_H
