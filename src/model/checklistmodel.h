#ifndef CHECKLISTMODEL_H
#define CHECKLISTMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QStandardItemModel>
#include <QSet>

class CheckListModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit CheckListModel(int columnCount, QObject *parent = nullptr);

public:

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    QList<int> checked() const;

    void selectAll();
    void unselectAll();

public:
    QSet<int> mChecked;

signals:

protected:
    void allDataChanged();
};

#endif // CHECKLISTMODEL_H
