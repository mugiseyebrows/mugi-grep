#include "model/editorsmodel.h"

#include <QFileInfo>

EditorsModel::EditorsModel(QObject *parent) :
    QStandardItemModel(100,2,parent)
{
}

QVariant EditorsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        static QStringList captions = {"Extensions", "Application"};
        return captions.value(section);
    }
    return QStandardItemModel::headerData(section,orientation,role);
}

bool EditorsModel::isEmpty() const
{
    for(int r=0;r<rowCount();r++) {
        for (int c=0;c<columnCount();c++) {
            if (!data(index(r,c)).toString().isEmpty()) {
                return false;
            }
        }
    }
    return true;
}

int EditorsModel::emptyRow() const {
    for(int r=0;r<rowCount();r++) {
        bool empty = true;
        for (int c=0;c<columnCount();c++) {
            if (!data(index(r,c)).toString().isEmpty()) {
                empty = false;
                break;
            }
        }
        if (empty) {
            return r;
        }
    }
    return -1;
}


QStringList EditorsModel::extGroup(const QString& ext) {
    static QList<QStringList> groups = {
        {"cpp","hpp","h","cc","cxx"},
        {"js","ts","cjs"},
        {"htm","html"}
    };
    QStringList result = {ext};
    for(const QStringList& group: groups) {
        if (group.contains(ext)) {
            result = group;
        }
    }
    return result;
}

int EditorsModel::append(const QString &path)
{
    int row = emptyRow();
    if (path.isEmpty()) {
        setData(index(row,0),".*");
        return row;
    }
    QFileInfo info(path);
    QString ext = info.suffix();
    QStringList group = extGroup(ext);
    setData(index(row,0),group.join("|"));
    return row;
}
