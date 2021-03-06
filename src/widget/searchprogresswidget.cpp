#include "searchprogresswidget.h"
#include "ui_searchprogresswidget.h"

SearchProgressWidget::SearchProgressWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchProgressWidget)
{
    ui->setupUi(this);
}

SearchProgressWidget::~SearchProgressWidget()
{
    delete ui;
}

void SearchProgressWidget::init() {
    ui->progressBar->hide();
    ui->cancel->hide();
    ui->status1->hide();
    ui->status2->setTextElideMode(Qt::ElideMiddle);
    ui->status2->hide();
    connect(ui->cancel,SIGNAL(clicked(bool)),this,SIGNAL(canceled()));
}

void SearchProgressWidget::progress(int i, int t, int s, QString path) {
    ui->status1->setText(QString("Processed %1 files out of %2 (%3 filtered out)").arg(i).arg(t).arg(s));
    if (i >= 0) {
        QString status2 = i == t ? QString() : QString("Last file: %1").arg(path);
        ui->status2->setText(status2);
        ui->status2->setVisible(!status2.isEmpty());
        ui->progressBar->setMaximum(t);
        ui->progressBar->setValue(i);
        ui->progressBar->setVisible( i != t );
    }
    ui->cancel->setVisible( i != t );
}

void SearchProgressWidget::started() {
    ui->status1->setText(QString("Building path list"));
    ui->status2->setText(QString());
    ui->status1->setVisible(true);
}

void SearchProgressWidget::aborted() {
    ui->progressBar->setVisible(false);
    ui->cancel->setVisible(false);
    ui->status1->setText(QString("Search aborted"));
    ui->status2->setText(QString());
    ui->status2->hide();
}

void SearchProgressWidget::replaced(int files,int lines) {
    ui->status1->setText(QString("%2 line%4 replaced in %1 file%3")
                         .arg(files)
                         .arg(lines)
                         .arg(files == 1 ? "" : "s")
                         .arg(lines == 1 ? "" : "s"));
    ui->status2->setText(QString());
    ui->status2->hide();
}

void SearchProgressWidget::renamed(int successful,int failed) {

    ui->status1->setText(QString("%1 file%2 renamed")
                         .arg(successful)
                         .arg(successful == 1 ? "" : "s"));
    ui->status2->setText(QString());
    ui->status2->hide();
}
