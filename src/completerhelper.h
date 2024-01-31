#ifndef COMPLETERHELPER_H
#define COMPLETERHELPER_H

class QStandardItemModel;
class QCompleter;
class QObject;
class QWidget;
#include <QStringList>

class CompleterHelper
{
public:

    static QStandardItemModel *pathsToModel(const QStringList &files, QObject *parent);
    static QCompleter *modelToCompleter(QStandardItemModel *model, int column, QWidget *parent);
    static void completerTreeViewPopup(QCompleter *completer, QWidget *parent);

};

#endif // COMPLETERHELPER_H
