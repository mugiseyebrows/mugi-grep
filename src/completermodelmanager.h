#ifndef COMPLETERMODELMANAGER_H
#define COMPLETERMODELMANAGER_H

#include <QObject>
#include <QTabWidget>
#include "widget/searchoptionswidget.h"

class SessionWidget;
class QStringListModel;

class CompleterModelManager : public QObject
{
    Q_OBJECT
public:
    explicit CompleterModelManager(QObject *parent = nullptr);
    SessionWidget *tab(QTabWidget *tabs, int index);

protected:

public slots:

    void onCollect(SearchOptionsWidget *options, QTabWidget *tabs);
};

#endif // COMPLETERMODELMANAGER_H
