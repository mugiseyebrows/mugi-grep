#include "searchbrowser.h"
#include <QDebug>

SearchBrowser::SearchBrowser(QWidget* parent)
    : QTextBrowser(parent), mSearchId(-1), mLinesBefore(0), mLinesAfter(0), mNotBinary(false),
      mChanged(QDateTime::currentDateTime()) {
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
}

RegExp SearchBrowser::exp() const {
    return mExp;
}

void SearchBrowser::setExp(const RegExp& exp) {
    mExp = exp;
    mChanged = QDateTime::currentDateTime();
}

RegExpPath SearchBrowser::filter() const {
    return mFilter;
}

void SearchBrowser::setFilter(const RegExpPath& filter) {
    mFilter = filter;
    mChanged = QDateTime::currentDateTime();
}

int SearchBrowser::linesBefore() const {
    return mLinesBefore;
}

void SearchBrowser::setLinesBefore(int linesBefore) {
    mLinesBefore = linesBefore;
    mChanged = QDateTime::currentDateTime();
}

int SearchBrowser::linesAfter() const {
    return mLinesAfter;
}

void SearchBrowser::setLinesAfter(int linesAfter) {
    mLinesAfter = linesAfter;
    mChanged = QDateTime::currentDateTime();
}

bool SearchBrowser::notBinary() const {
    return mNotBinary;
}

void SearchBrowser::setNotBinary(bool notBinary) {
    mNotBinary = notBinary;
    mChanged = QDateTime::currentDateTime();
}

QDateTime SearchBrowser::changed() const {
    return mChanged;
}

void SearchBrowser::setChanged(const QDateTime& changed) {
    mChanged = changed;
}

QString SearchBrowser::replacement() const {
    return mReplacement;
}

void SearchBrowser::setReplacement(const QString& replacement) {
    mReplacement = replacement;
    mChanged = QDateTime::currentDateTime();
}

bool SearchBrowser::preserveCase() const {
    return mPreserveCase;
}

void SearchBrowser::setPreserveCase(bool preserveCase) {
    mPreserveCase = preserveCase;
    mChanged = QDateTime::currentDateTime();
}

bool SearchBrowser::isExecuted() const {
    return mSearchId > -1;
}

void SearchBrowser::copy(SearchBrowser* dest) {
    dest->setExp(mExp);
    dest->setFilter(mFilter);
    dest->setLinesBefore(mLinesBefore);
    dest->setLinesAfter(mLinesAfter);
    dest->setNotBinary(mNotBinary);
    dest->setChanged(mChanged);
    dest->setReplacement(mReplacement);
    dest->setPreserveCase(mPreserveCase);
}

SearchParams SearchBrowser::params(int action, int id, const QString& path, bool cacheFileList) {
    return SearchParams(action, id, path, mFilter, mNotBinary, mExp, mLinesBefore, mLinesAfter,
                        cacheFileList, mPreserveCase, mReplacement);
}
