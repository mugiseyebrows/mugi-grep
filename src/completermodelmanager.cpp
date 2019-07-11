#include "completermodelmanager.h"
#include "widget/sessionwidget.h"

#include "rxcollector.h"
#include "widget/searchoptionswidget.h"

CompleterModelManager::CompleterModelManager(QObject *parent) : QObject(parent)
{

}

void CompleterModelManager::onCollect(SearchOptionsWidget* options, QTabWidget * tabs)
{
    QList<QStringListModel*> models = RXCollector::instance()->models();
    options->collect();
    for(int index=0;index<tabs->count();index++) {
        SessionWidget* session = tab(tabs, index);
        if (!session) {
            qDebug() << "not SessionWidget at index" << index << tabs->widget(index);
            return;
        }
        session->updateCompletions();
    }
    foreach (QStringListModel* model, models) {
        model->deleteLater();
    }
}

SessionWidget *CompleterModelManager::tab(QTabWidget* tabs, int index)
{
    return qobject_cast<SessionWidget*>(tabs->widget(index));
}
