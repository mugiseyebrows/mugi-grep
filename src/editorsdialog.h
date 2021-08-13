#ifndef EDITORSDIALOG_H
#define EDITORSDIALOG_H

#include <QDialog>
#include "editor.h"

namespace Ui {
class EditorsDialog;
}

class EditorsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditorsDialog(QWidget *parent = nullptr);
    ~EditorsDialog();

    Editor editor() const;

    void showAssociate(bool value);

    bool associate() const;

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_select_clicked();

protected:
    Editor mEditor;
    QList<Editor> mDetected;
    Ui::EditorsDialog *ui;
};

#endif // EDITORSDIALOG_H
