#include "model/editorsmodel.h"

EditorsModel::EditorsModel(QObject *parent) :
    QStandardItemModel(10,2,parent)
{
}

QVariant EditorsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0) {
            return "exts";
        } else if (section == 1) {
            return "app";
        }
    }
    return QStandardItemModel::headerData(section,orientation,role);
}
