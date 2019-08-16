#include "searchparams.h"

SearchParams::SearchParams() {
}

SearchParams::SearchParams(int action, int id, const QString& path, bool cacheFileList,
                           const RegExpPath& filter, bool skipBinary, const RegExp& search,
                           int linesBefore, int linesAfter, bool showFileName, bool showLineNumber,
                           bool onlyMatched, const QString& replace, bool preserveCase)
    : mAction(action), mId(id), mPath(path), mCacheFileList(cacheFileList), mFilter(filter),
      mSkipBinary(skipBinary), mSearch(search), mLinesBefore(linesBefore), mLinesAfter(linesAfter),
      mShowFileName(showFileName), mShowLineNumber(showLineNumber), mOnlyMatched(onlyMatched),
      mReplace(replace), mPreserveCase(preserveCase) {
}

int SearchParams::action() const {
    return mAction;
}

int SearchParams::id() const {
    return mId;
}

QString SearchParams::path() const {
    return mPath;
}

bool SearchParams::cacheFileList() const {
    return mCacheFileList;
}

RegExpPath SearchParams::filter() const {
    return mFilter;
}

bool SearchParams::skipBinary() const {
    return mSkipBinary;
}

RegExp SearchParams::search() const {
    return mSearch;
}

int SearchParams::linesBefore() const {
    return mLinesBefore;
}

int SearchParams::linesAfter() const {
    return mLinesAfter;
}

bool SearchParams::showFileName() const {
    return mShowFileName;
}

bool SearchParams::showLineNumber() const {
    return mShowLineNumber;
}

bool SearchParams::onlyMatched() const {
    return mOnlyMatched;
}

QString SearchParams::replace() const {
    return mReplace;
}

bool SearchParams::preserveCase() const {
    return mPreserveCase;
}
