#include "model/checkablestringlistmodel.h"

CheckableStringListModel::CheckableStringListModel(const QStringList &strings, QObject *parent)
    : QStringListModel(strings,parent)
{

}

QVariant CheckableStringListModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::CheckStateRole) {
        if (index.column() == 0) {
            return mChecked.contains(index.row()) ? Qt::Checked : Qt::Unchecked;
        }
    }
    return QStringListModel::data(index,role);
}

bool CheckableStringListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole) {
        if (index.column() == 0) {
            if (value.toInt() == Qt::Checked) {
                mChecked.insert(index.row());
            } else if (value.toInt() == Qt::Unchecked) {
                mChecked.remove(index.row());
            }
            emit dataChanged(index,index);
        }
    }
    return QStringListModel::setData(index,value,role);
}

Qt::ItemFlags CheckableStringListModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0) {
        return QStringListModel::flags(index) | Qt::ItemIsUserCheckable;
    }
    return QStringListModel::flags(index);
}

QStringList CheckableStringListModel::checked() const
{
    return checkState(Qt::Checked);
}

QStringList CheckableStringListModel::unchecked() const
{
    return checkState(Qt::Unchecked);
}

QStringList CheckableStringListModel::checkState(Qt::CheckState state) const
{
    QStringList res;
    for(int r=0;r<rowCount();r++) {
        if (data(index(r,0),Qt::CheckStateRole).toInt() == state) {
            res << data(index(r,0)).toString();
        }
    }
    return res;
}

bool CheckableStringListModel::hasAnyChecked() const
{
    return hasAnyCheckState(Qt::Checked);
}

bool CheckableStringListModel::hasAnyUnchecked() const
{
    return hasAnyCheckState(Qt::Unchecked);
}

bool CheckableStringListModel::hasAnyCheckState(Qt::CheckState state) const
{
    for(int r=0;r<rowCount();r++) {
        if (data(index(r,0),Qt::CheckStateRole).toInt() == state) {
            return true;
        }
    }
    return false;
}

void CheckableStringListModel::setAllChecked()
{
    setAllCheckState(Qt::Checked);
}

void CheckableStringListModel::setAllUnchecked()
{
    setAllCheckState(Qt::Unchecked);
}

void CheckableStringListModel::setAllCheckState(Qt::CheckState state)
{
    for(int r=0;r<rowCount();r++) {
        setData(index(r,0),state,Qt::CheckStateRole);
    }
}
