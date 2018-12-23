#include "widget/searchbrowser.h"

#include <QFont>
#include <QDebug>

SearchBrowser::SearchBrowser(RegExp exp, RegExpPath filter, int linesBefore, int linesAfter, bool cacheFileList, QWidget *parent) :
    QTextBrowser(parent), mExp(exp), mFilter(filter), mLinesBefore(linesBefore), mLinesAfter(linesAfter), mCacheFileList(cacheFileList)
{
    setOpenLinks(false);

    //@todo external stylesheet

#ifdef Q_OS_LINUX
    QFont font("Liberation Mono",11,QFont::Normal);
#endif

#ifdef Q_OS_WIN
    QFont font("Courier New",12,QFont::Normal);
#endif

    setFont(font);
}

SearchBrowser::~SearchBrowser()
{
    qDebug() << "~SearchBrowser()";
}

RegExp SearchBrowser::exp() const
{
    return mExp;
}

RegExpPath SearchBrowser::filter() const
{
    return mFilter;
}

int SearchBrowser::linesBefore() const
{
    return mLinesBefore;
}

int SearchBrowser::linesAfter() const
{
    return mLinesAfter;
}

bool SearchBrowser::cacheFileList() const
{
    return mCacheFileList;
}
