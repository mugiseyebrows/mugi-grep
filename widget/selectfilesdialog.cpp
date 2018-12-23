#include "selectfilesdialog.h"
#include "ui_selectfilesdialog.h"

#include "worker.h"
#include "html.h"

#include "anchorclickhandler.h"
#include "rxcollector.h"
#include "searchcache.h"
#include "model/checkablestringlistmodel.h"

SelectFilesDialog::SelectFilesDialog(const QString& path, RegExpPath filter,
                                     Worker* worker, AnchorClickHandler *clickHandler,
                                     QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectFilesDialog)
{
    ui->setupUi(this);

// todo move to static function
#ifdef Q_OS_LINUX
    QFont font("Liberation Mono",11,QFont::Normal);
#endif

#ifdef Q_OS_WIN
    QFont font("Courier New",12,QFont::Normal);
#endif

    ui->files->setOpenLinks(false);
    ui->files->setFont(font);

    connect(this,SIGNAL(getAllFiles(QString)),worker,SLOT(onGetAllFiles(QString)));
    connect(worker,SIGNAL(allFiles(QString,QStringList)),this,SLOT(onAllFiles(QString,QStringList)));
    emit getAllFiles(path);

    clickHandler->setBrowser(ui->files);

    RXCollector::instance()->load(ui->filter);

    ui->filter->setValue(filter);
}

SelectFilesDialog::~SelectFilesDialog()
{
    delete ui;
}

RegExpPath SelectFilesDialog::filter() const
{
    return ui->filter->value();
}

void SelectFilesDialog::showInBrowser(const QString& path, const QStringList &files)
{
    QDir dir(path);
    QStringList res;
    foreach(const QString file, files) {
        QString relPath = dir.relativeFilePath(file);
        QString href = "file:///" + QDir::toNativeSeparators(file);
        res << Html::anchor(relPath,href,"violet");
    }
    ui->files->setText(res.join("<br/>"));
}

void SelectFilesDialog::onAllFiles(QString path,QStringList files) {

    mPath = path;
    mFiles = files;
    //showInBrowser(path, files);

    QMap<QString,int> stat;

    foreach(const QString file, files) {
        QString ext = RegExpPath::getExt(file);
        if (!ext.isEmpty()) {
            if (!stat.contains(ext)) {
                stat[ext] = 0;
            }
            stat[ext] += 1;
        }
    }

    QStringList exts = stat.keys();
    qSort(exts.begin(),exts.end());

    QStringList data;
    foreach(const QString& ext, exts) {
        data << QString("%1 (%2)").arg(ext).arg(stat[ext]);
    }
    CheckableStringListModel* model = new CheckableStringListModel(data);
    ui->extensions->setModel(model);

    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(onExtensionsDataChanged(QModelIndex,QModelIndex)));

    on_filter_textChanged();
}

namespace  {

QStringList mapSplitColumn(const QStringList& items, const QRegularExpression sep, int col) {
    QStringList res;
    foreach(const QString item, items) {
        QStringList cols = item.split(sep);
        res << cols.value(col);
    }
    return res;
}

}

void SelectFilesDialog::onExtensionsDataChanged(QModelIndex,QModelIndex) {

    CheckableStringListModel* model = qobject_cast<CheckableStringListModel*>(ui->extensions->model());
    if (!model) {
        return;
    }
    QStringList exts = mapSplitColumn(model->checked(),QRegularExpression("\\s+"),0);
    //ui->filter->setIn

    ui->filter->setIncludeExtValue(exts.join("|"));

}

void SelectFilesDialog::on_filter_textChanged()
{
    //qDebug() << "on_filter_textChanged";

    if (mFiles.isEmpty()) {
        return;
    }

    int filesFiltered;
    int dirsFiltered;

    QStringList files = SearchCache::filterFiles(mFiles,ui->filter->value(),false,&filesFiltered,&dirsFiltered);
    showInBrowser(mPath,files);

}
