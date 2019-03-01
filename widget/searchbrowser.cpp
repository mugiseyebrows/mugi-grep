#include "searchbrowser.h"
#include <QDebug>

SearchBrowser::SearchBrowser(QWidget* parent)
    : QTextBrowser(parent), mSearchId(-1), mLinesBefore(0), mLinesAfter(0), mCacheFileList(false),
      mNotBinary(false), mChanged(QDateTime::currentDateTime()) {
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
int SearchBrowser::searchId() const {
    return mSearchId;
}

void SearchBrowser::setSearchId(int searchId) {
    mSearchId = searchId;
    mChanged = QDateTime::currentDateTime();
    qDebug() << "searchId changed";
}

RegExp SearchBrowser::exp() const {
    return mExp;
}

void SearchBrowser::setExp(const RegExp& exp) {
    mExp = exp;
    mChanged = QDateTime::currentDateTime();
    qDebug() << "exp changed";
}

RegExpPath SearchBrowser::filter() const {
    return mFilter;
}

void SearchBrowser::setFilter(const RegExpPath& filter) {
    mFilter = filter;
    mChanged = QDateTime::currentDateTime();
    qDebug() << "filter changed";
}

int SearchBrowser::linesBefore() const {
    return mLinesBefore;
}

void SearchBrowser::setLinesBefore(int linesBefore) {
    mLinesBefore = linesBefore;
    mChanged = QDateTime::currentDateTime();
    qDebug() << "linesBefore changed";
}

int SearchBrowser::linesAfter() const {
    return mLinesAfter;
}

void SearchBrowser::setLinesAfter(int linesAfter) {
    mLinesAfter = linesAfter;
    mChanged = QDateTime::currentDateTime();
    qDebug() << "linesAfter changed";
}

bool SearchBrowser::cacheFileList() const {
    return mCacheFileList;
}

void SearchBrowser::setCacheFileList(bool cacheFileList) {
    mCacheFileList = cacheFileList;
    mChanged = QDateTime::currentDateTime();
    qDebug() << "cacheFileList changed";
}

bool SearchBrowser::notBinary() const {
    return mNotBinary;
}

void SearchBrowser::setNotBinary(bool notBinary) {
    mNotBinary = notBinary;
    mChanged = QDateTime::currentDateTime();
    qDebug() << "notBinary changed";
}

QDateTime SearchBrowser::changed() const {
    return mChanged;
}

void SearchBrowser::setChanged(const QDateTime& changed) {
    mChanged = changed;
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
    dest->setNotBinary(mNotBinary);
    dest->setChanged(mChanged);
}
