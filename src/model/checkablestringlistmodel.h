#ifndef CHECKABLESTRINGLISTMODEL_H
#define CHECKABLESTRINGLISTMODEL_H

#include <QObject>
#include <QStringListModel>
#include <QSet>

class CheckableStringListModel : public QStringListModel
{
    Q_OBJECT
public:
    CheckableStringListModel(const QStringList &strings, QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QStringList checked() const;
    QStringList unchecked() const;
    QStringList checkState(Qt::CheckState state) const;

    bool hasAnyChecked() const;
    bool hasAnyUnchecked() const;
    bool hasAnyCheckState(Qt::CheckState state) const;

    void setAllChecked();
    void setAllUnchecked();
    void setAllCheckState(Qt::CheckState state);

protected:
    QSet<int> mChecked;

};

#endif // CHECKABLESTRINGLISTMODEL_H
