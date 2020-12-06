#include "checklistmodel.h"


namespace {
int tableItemCount(const QModelIndex &parent, int count) {
    if (parent.isValid()) {
        return 0;
    }
    return count;
}
}

CheckListModel::CheckListModel(int columnCount, QObject *parent) : QStandardItemModel(0, columnCount, parent)
{
    setRowCount(10);
}

QVariant CheckListModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::CheckStateRole && index.column() == 0) {
        if (index.column() == 0) {
            return mChecked.contains(index.row()) ? Qt::Checked : Qt::Unchecked;
        }
    }
    return QStandardItemModel::data(index, role);
}

Qt::ItemFlags CheckListModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0) {
        return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool CheckListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }
    if (role == Qt::CheckStateRole && index.column() == 0) {
        if (value.toInt() == Qt::Checked) {
            mChecked.insert(index.row());
        } else {
            mChecked.remove(index.row());
        }
        emit dataChanged(index,index);
        return true;
    }
    return QStandardItemModel::setData(index, value, role);
}

QList<int> CheckListModel::checked() const
{
    QList<int> res;
    for(int r=0;r<rowCount();r++) {
        if (mChecked.contains(r)) {
            res.append(r);
        }
    }
    return res;
}

void CheckListModel::selectAll()
{
    for(int r=0;r<rowCount();r++) {
        mChecked.insert(r);
    }
    allDataChanged();
}

void CheckListModel::unselectAll()
{
    mChecked.clear();
    allDataChanged();
}

void CheckListModel::allDataChanged() {
    emit dataChanged(index(0,0),index(rowCount()-1,columnCount()-1));
}

