#include "renamedialog.h"
#include "ui_renamedialog.h"

#include "model/checklistmodel.h"
#include "fileio.h"

RenameDialog::RenameDialog(const QList<QPair<QString,QString>> &files, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenameDialog), mFiles(files)
{
    ui->setupUi(this);

    CheckListModel* model = new CheckListModel(3, this);
    model->setRowCount(files.size());
    int row = 0;

    model->setHorizontalHeaderLabels(QStringList {"Name", "New name", "Path"});

    for(const QPair<QString,QString>& file: files) {

        QString name = FileIO::nameFromPath(file.first);
        QString newName = FileIO::nameFromPath(file.second);
        QString dirName = FileIO::dirName(file.first);

        model->setData(model->index(row, 0), Qt::Checked, Qt::CheckStateRole);
        model->setData(model->index(row, 0), name);
        model->setData(model->index(row, 1), newName);
        model->setData(model->index(row, 2), dirName);

        row++;
    }

    ui->tableView->setModel(model);

}

RenameDialog::~RenameDialog()
{
    delete ui;
}

QList<QPair<QString, QString> > RenameDialog::checked() const
{
    CheckListModel* model = qobject_cast<CheckListModel*>(ui->tableView->model());
    QList<int> checked = model->checked();
    QList<QPair<QString, QString> > res;
    for(int row: checked) {
        res << mFiles[row];
    }
    return res;
}

void RenameDialog::on_all_clicked()
{
    CheckListModel* model = qobject_cast<CheckListModel*>(ui->tableView->model());
    model->selectAll();
}

void RenameDialog::on_none_clicked()
{
    CheckListModel* model = qobject_cast<CheckListModel*>(ui->tableView->model());
    model->unselectAll();
}
