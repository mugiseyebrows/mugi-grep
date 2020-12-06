#include "countfilesmanager.h"

CountFilesManager::CountFilesManager(QObject *parent) : QObject(parent)
{

}

QPair<int, int> CountFilesManager::count(const QString &path, const RegExpPath &filter)
{
    foreach(const CountFilesParams& item, mData) {
        if (item.path() == path && item.filter() == filter) {
            return {item.filtered(), item.total()};
        }
    }
    CountFilesParams params(path, filter, -1, -1);
    mData.append(params);
    emit countFiles(params);
    return {-1,-1};
}

void CountFilesManager::onFilesCounted(CountFilesParams params)
{
    for(int i=0;i<mData.size();i++) {
        const CountFilesParams& item = mData[i];
        if (item.path() == params.path() && item.filter() == params.filter()) {
            mData[i] = params;
        }
    }
    emit filesCounted();
}
