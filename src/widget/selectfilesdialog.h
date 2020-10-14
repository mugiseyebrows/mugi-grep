#ifndef SELECTFILESDIALOG_H
#define SELECTFILESDIALOG_H

#include <QDialog>
#include <QModelIndex>

#include "regexppath.h"

#if 0

class Worker;

namespace Ui {
class SelectFilesDialog;
}

class AnchorClickHandler;
class CallOnce;

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
    CallOnce* mFilterFiles;

    void showInBrowser(const QString &path, const QStringList &files);

signals:
    void getAllFiles(QString);

protected slots:
    void onExtensionsDataChanged(QModelIndex, QModelIndex);
    void onAllFiles(QString path, QStringList files);

    void onFilterFiles();
private:
    Ui::SelectFilesDialog *ui;
};

#endif

#endif // SELECTFILESDIALOG_H
