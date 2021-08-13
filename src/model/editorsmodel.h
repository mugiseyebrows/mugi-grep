#ifndef EDITORSMODEL_H
#define EDITORSMODEL_H

#include <QStandardItemModel>

class EditorsModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit EditorsModel(QObject *parent = 0);
    
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool isEmpty() const;

    int append(const QString& path);

    int emptyRow() const;

    static QStringList extGroup(const QString &ext);
signals:
    
public slots:
    
};

#endif // EDITORSMODEL_H
