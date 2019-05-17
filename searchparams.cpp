#include "searchparams.h"

SearchParams::SearchParams()
    : mAction(-1), mId(-1), mLinesBefore(-1), mLinesAfter(-1), mFilesComplete(-1),
      mFilesFiltered(-1), mDirsFiltered(-1) {
}

SearchParams::SearchParams(int action, int id, const QString& path, const RegExpPath& filter,
                           bool skipBinary, const RegExp& search, int linesBefore, int linesAfter,
                           bool cacheFileList, const QString& replace)
    : mAction(action), mId(id), mPath(path), mFilter(filter), mSkipBinary(skipBinary),
      mSearch(search), mLinesBefore(linesBefore), mLinesAfter(linesAfter),
      mCacheFileList(cacheFileList), mReplace(replace), mFilesComplete(0), mFilesFiltered(0),
      mDirsFiltered(0) {
}

int SearchParams::action() const {
    return mAction;
}

void SearchParams::setAction(int action) {
    mAction = action;
}

int SearchParams::id() const {
    return mId;
}

void SearchParams::setId(int id) {
    mId = id;
}

QString SearchParams::path() const {
    return mPath;
}

void SearchParams::setPath(const QString& path) {
    mPath = path;
}

RegExpPath SearchParams::filter() const {
    return mFilter;
}

void SearchParams::setFilter(const RegExpPath& filter) {
    mFilter = filter;
}

bool SearchParams::skipBinary() const {
    return mSkipBinary;
}

void SearchParams::setSkipBinary(bool skipBinary) {
    mSkipBinary = skipBinary;
}

RegExp SearchParams::search() const {
    return mSearch;
}

void SearchParams::setSearch(const RegExp& search) {
    mSearch = search;
}

int SearchParams::linesBefore() const {
    return mLinesBefore;
}

void SearchParams::setLinesBefore(int linesBefore) {
    mLinesBefore = linesBefore;
}

int SearchParams::linesAfter() const {
    return mLinesAfter;
}

void SearchParams::setLinesAfter(int linesAfter) {
    mLinesAfter = linesAfter;
}

bool SearchParams::cacheFileList() const {
    return mCacheFileList;
}

void SearchParams::setCacheFileList(bool cacheFileList) {
    mCacheFileList = cacheFileList;
}

QString SearchParams::replace() const {
    return mReplace;
}

void SearchParams::setReplace(const QString& replace) {
    mReplace = replace;
}

QStringList SearchParams::files() const {
    return mFiles;
}

void SearchParams::setFiles(const QStringList& files) {
    mFiles = files;
}

int SearchParams::filesComplete() const {
    return mFilesComplete;
}

void SearchParams::setFilesComplete(int filesComplete) {
    mFilesComplete = filesComplete;
}

int SearchParams::filesFiltered() const {
    return mFilesFiltered;
}

void SearchParams::setFilesFiltered(int filesFiltered) {
    mFilesFiltered = filesFiltered;
}

int SearchParams::dirsFiltered() const {
    return mDirsFiltered;
}

void SearchParams::setDirsFiltered(int dirsFiltered) {
    mDirsFiltered = dirsFiltered;
}

int SearchParams::filesSize() const {
    return mFiles.size();
}

QString SearchParams::file(int i) const {
    return mFiles[i];
}
