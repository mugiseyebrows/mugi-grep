#ifndef EDITORSMODEL_H
#define EDITORSMODEL_H

#include <QStandardItemModel>

class EditorsModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit EditorsModel(QObject *parent = 0);
    
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

signals:
    
public slots:
    
};

#endif // EDITORSMODEL_H
