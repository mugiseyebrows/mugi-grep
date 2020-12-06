#ifndef RENAMEDIALOG_H
#define RENAMEDIALOG_H

#include <QDialog>
#include <QList>
#include <QPair>

namespace Ui {
class RenameDialog;
}

class RenameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RenameDialog(const QList<QPair<QString,QString>>& files, QWidget *parent = nullptr);
    ~RenameDialog();

    QList<QPair<QString,QString>> checked() const;

protected:
    QList<QPair<QString,QString>> mFiles;

private slots:
    void on_all_clicked();

    void on_none_clicked();

private:
    Ui::RenameDialog *ui;
};

#endif // RENAMEDIALOG_H
