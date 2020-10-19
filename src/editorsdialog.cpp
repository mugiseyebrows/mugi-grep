#include "editorsdialog.h"
#include "ui_editorsdialog.h"

#include "editordetector.h"
#include <QStringListModel>

EditorsDialog::EditorsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditorsDialog)
{
    ui->setupUi(this);

    mDetected = EditorDetector::detect();

    QStringList names;
    for(const Editor& editor: mDetected) {
        names << editor.name();
    }
    QStringListModel* m = new QStringListModel(names);
    ui->tableView->setModel(m);
}

EditorsDialog::~EditorsDialog()
{
    delete ui;
}

Editor EditorsDialog::editor() const
{
    if (!mEditor.isNull()) {
        return mEditor;
    }
    QModelIndex index = ui->tableView->currentIndex();
    if (index.isValid()) {
        int row = index.row();
        return mDetected[row];
    }
    return Editor();
}

void EditorsDialog::on_tableView_doubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    int row = index.row();
    mEditor = mDetected[row];
    accept();
}

namespace  {

QString quoted(const QString& path)
{
    if (path.indexOf(" ") > -1) {
        return "\"" + path + "\"";
    }
    return path;
}

QString joinSpace(const QStringList& args) {
    return args.join(" ");
}

}


#include <QFileDialog>

void EditorsDialog::on_select_clicked()
{
    QString filter;
#if defined(Q_OS_WIN32)
    filter = "Executables (*.exe)";
#endif

    QString path = QFileDialog::getOpenFileName(this, QString(), QString(), filter);
    if (!path.isEmpty()) {
        mEditor = Editor("", joinSpace({quoted(QDir::toNativeSeparators(path)),"%file%"}));
        accept();
    }
}
