#include "searchoptionswidget.h"
#include "ui_searchoptionswidget.h"

#include "searchbrowser.h"
#include "rxcollector.h"
#include <QDebug>
#include <QFileDialog>
#include <QDir>

SearchOptionsWidget::SearchOptionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchOptionsWidget)
{
    ui->setupUi(this);

    connect(ui->filter,SIGNAL(returnPressed()),this,SIGNAL(search()));
    connect(ui->exp,SIGNAL(returnPressed()),this,SIGNAL(search()));

    connect(ui->filter,SIGNAL(textChanged()),this,SLOT(onFilterTextChanged()));
    connect(ui->exp,SIGNAL(textChanged()),this,SLOT(onExpTextChanged()));
    connect(ui->linesAfter,SIGNAL(valueChanged(int)),this,SLOT(onLinesAfterValueChanged()));
    connect(ui->linesBefore,SIGNAL(valueChanged(int)),this,SLOT(onLinesBeforeValueChanged()));
    connect(ui->cacheFileList,SIGNAL(clicked(bool)),this,SLOT(onCacheFileListClicked(bool)));
}

SearchOptionsWidget::~SearchOptionsWidget()
{
    delete ui;
}

void SearchOptionsWidget::setBrowser(SearchBrowser *browser, bool setValues)
{
    qDebug() << "setBrowser" << browser << setValues;

    mBrowser = browser;
    mActive = false;

    if (setValues) {

        RXCollector::instance()->load(ui->filter);
        RXCollector::instance()->load(ui->exp);

        ui->filter->setValue(browser->filter());
        ui->exp->setValue(browser->exp());
        ui->linesBefore->setValue(browser->linesBefore());
        ui->linesAfter->setValue(browser->linesAfter());
        ui->cacheFileList->setChecked(browser->cacheFileList());

        //ui->path->setText(path);
    }

    mActive = true;
}

QString SearchOptionsWidget::path() const
{
    return ui->path->text();
}

void SearchOptionsWidget::setPath(const QString &path)
{
    ui->path->setText(path);
}

void SearchOptionsWidget::setBrowserValues()
{
    mBrowser->setFilter(ui->filter->value());
    mBrowser->setExp(ui->exp->value());
    mBrowser->setLinesBefore(ui->linesBefore->value());
    mBrowser->setLinesAfter(ui->linesAfter->value());
    mBrowser->setCacheFileList(ui->cacheFileList->isChecked());
}

void SearchOptionsWidget::collect()
{
    RXCollector* collector = RXCollector::instance();
    mActive = false;
    collector->collect(ui->exp->value());
    collector->collect(ui->filter->value());
    collector->load(ui->exp);
    collector->load(ui->filter);
    mActive = true;
}

void SearchOptionsWidget::setActive(bool active)
{
    mActive = active;
}


void SearchOptionsWidget::on_selectPath_clicked()
{

    QString path = QFileDialog::getExistingDirectory(this, QString(), ui->path->text());
    if (path.isEmpty()) {
        return;
    }
    ui->path->setText(QDir::toNativeSeparators(path));
}

void SearchOptionsWidget::on_search_clicked()
{
    emit search();
}

void SearchOptionsWidget::onFilterTextChanged() {
    if (!mActive || !mBrowser) {
        return;
    }
    if (mBrowser->isExecuted()) {
        emit clone();
        return;
    }
    mBrowser->setFilter(ui->filter->value());
}

void SearchOptionsWidget::onExpTextChanged() {
    if (!mActive || !mBrowser) {
        return;
    }
    if (mBrowser->isExecuted()) {
        emit clone();
        return;
    }
    mBrowser->setExp(ui->exp->value());
    emitTabTitle();
}

void SearchOptionsWidget::emitTabTitle() {
    emit tabTitle(ui->exp->value().include(), mBrowser->isExecuted());
}

void SearchOptionsWidget::onLinesAfterValueChanged() {
    if (!mActive || !mBrowser) {
        return;
    }
    if (mBrowser->isExecuted()) {
        emit clone();
        return;
    }
    mBrowser->setLinesAfter(ui->linesAfter->value());
}

void SearchOptionsWidget::onLinesBeforeValueChanged() {
    if (!mActive || !mBrowser) {
        return;
    }
    if (mBrowser->isExecuted()) {
        emit clone();
        return;
    }
    mBrowser->setLinesBefore(ui->linesBefore->value());
}

void SearchOptionsWidget::onCacheFileListClicked(bool value) {
    if (!mActive || !mBrowser) {
        return;
    }
    if (mBrowser->isExecuted()) {
        emit clone();
        return;
    }
    mBrowser->setCacheFileList(value);
}
