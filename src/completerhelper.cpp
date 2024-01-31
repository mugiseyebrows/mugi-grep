#include "completerhelper.h"

#include <QStandardItemModel>
#include <QCompleter>
#include <QTreeView>
#include <QHeaderView>
#include "fileio.h"

QStandardItemModel* CompleterHelper::pathsToModel(const QStringList& files, QObject* parent) {
    QStandardItemModel* model = new QStandardItemModel(files.size(), 2, parent);
    for(int r=0;r<model->rowCount();r++) {
        model->setData(model->index(r,0), FileIO::nameFromPath(files[r]));
        model->setData(model->index(r,1), files[r]);
    }
    return model;
}



QCompleter* CompleterHelper::modelToCompleter(QStandardItemModel* model, int column, QWidget* parent) {

    QCompleter* completer = new QCompleter(parent);

    completer->setModel(model);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionColumn(column);

    return completer;
}

void CompleterHelper::completerTreeViewPopup(QCompleter* completer, QWidget* parent) {
    QTreeView* view = new QTreeView(parent);
    completer->setPopup(view);
    view->header()->hide();
    view->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    view->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    view->header()->setStretchLastSection(false);
    view->setRootIsDecorated(false);

}
