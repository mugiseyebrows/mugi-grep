#ifndef SELECTFILESDIALOG_H
#define SELECTFILESDIALOG_H

#include <QDialog>
#include <QModelIndex>

#include "regexppath.h"

class Worker;

namespace Ui {
class SelectFilesDialog;
}

class AnchorClickHandler;

class SelectFilesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectFilesDialog(const QString &path, RegExpPath filter,
                               Worker *worker, AnchorClickHandler* clickHandler,
                               QWidget *parent = nullptr);
    ~SelectFilesDialog();

    RegExpPath filter() const;

protected:
    QStringList mFiles;
    QString mPath;

    void showInBrowser(const QString &path, const QStringList &files);

signals:
    void getAllFiles(QString);

protected slots:
    void onExtensionsDataChanged(QModelIndex, QModelIndex);
    void onAllFiles(QString path, QStringList files);
    void on_filter_textChanged();
private:
    Ui::SelectFilesDialog *ui;
};

#endif // SELECTFILESDIALOG_H
