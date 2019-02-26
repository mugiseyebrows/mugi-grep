#include "searchbrowser.h"
#include <QDebug>

SearchBrowser::SearchBrowser(QWidget* parent) :
    QTextBrowser(parent), mSearchId(-1), mCacheFileList(false), mLinesBefore(0), mLinesAfter(0) {
    setOpenLinks(false);
//@todo external stylesheet
#ifdef Q_OS_LINUX
    QFont font("Liberation Mono", 11, QFont::Normal);
#endif
#ifdef Q_OS_WIN
    QFont font("Courier New", 12, QFont::Normal);
#endif
    setFont(font);
}

SearchBrowser::~SearchBrowser() {
    qDebug() << "~SearchBrowser()";
}
RegExp SearchBrowser::exp() const {
    return mExp;
}

void SearchBrowser::setExp(const RegExp& exp) {
    mExp = exp;
}

RegExpPath SearchBrowser::filter() const {
    return mFilter;
}

void SearchBrowser::setFilter(const RegExpPath& filter) {
    mFilter = filter;
}

int SearchBrowser::linesBefore() const {
    return mLinesBefore;
}

void SearchBrowser::setLinesBefore(int linesBefore) {
    mLinesBefore = linesBefore;
}

int SearchBrowser::linesAfter() const {
    return mLinesAfter;
}

void SearchBrowser::setLinesAfter(int linesAfter) {
    mLinesAfter = linesAfter;
}

bool SearchBrowser::cacheFileList() const {
    return mCacheFileList;
}

void SearchBrowser::setCacheFileList(bool cacheFileList) {
    mCacheFileList = cacheFileList;
}

int SearchBrowser::searchId() const {
    return mSearchId;
}

void SearchBrowser::setSearchId(int searchId) {
    mSearchId = searchId;
}

bool SearchBrowser::isExecuted() const {
    return mSearchId > -1;
}

void SearchBrowser::copy(SearchBrowser* dest) {
    dest->setExp(mExp);
    dest->setFilter(mFilter);
    dest->setLinesBefore(mLinesBefore);
    dest->setLinesAfter(mLinesAfter);
    dest->setCacheFileList(mCacheFileList);
}
