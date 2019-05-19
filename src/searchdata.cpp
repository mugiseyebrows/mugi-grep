#include "searchdata.h"

SearchData::SearchData() : mFilesComplete(-1), mFilesFiltered(-1), mDirsFiltered(-1) {
}

SearchData::SearchData(const QStringList& files, int filesFiltered, int dirsFiltered)
    : mFiles(files), mFilesFiltered(filesFiltered), mDirsFiltered(dirsFiltered) {
}

QStringList SearchData::files() const {
    return mFiles;
}

void SearchData::setFiles(const QStringList& files) {
    mFiles = files;
}

int SearchData::filesComplete() const {
    return mFilesComplete;
}

void SearchData::setFilesComplete(int filesComplete) {
    mFilesComplete = filesComplete;
}

int SearchData::filesFiltered() const {
    return mFilesFiltered;
}

void SearchData::setFilesFiltered(int filesFiltered) {
    mFilesFiltered = filesFiltered;
}

int SearchData::dirsFiltered() const {
    return mDirsFiltered;
}

void SearchData::setDirsFiltered(int dirsFiltered) {
    mDirsFiltered = dirsFiltered;
}

int SearchData::filesSize() const {
    return mFiles.size();
}

QString SearchData::file(int i) const {
    return mFiles[i];
}
