#include "searchoptionswidget.h"
#include "ui_searchoptionswidget.h"

#include "searchbrowser.h"

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

#include "rxcollector.h"

void SearchOptionsWidget::setBrowser(SearchBrowser *browser, bool setValues)
{
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

void SearchOptionsWidget::setActive(bool active)
{
    mActive = active;
}

void SearchOptionsWidget::on_selectPath_clicked()
{
    if (!mActive) {
        return;
    }
}

void SearchOptionsWidget::on_search_clicked()
{
    emit search();
}

void SearchOptionsWidget::onFilterTextChanged() {
    if (!mActive || !mBrowser) {
        return;
    }
    if (mBrowser->executed()) {
        emit clone();
        return;
    }
    mBrowser->setFilter(ui->filter->value());
}

void SearchOptionsWidget::onExpTextChanged() {
    if (!mActive || !mBrowser) {
        return;
    }
    if (mBrowser->executed()) {
        emit clone();
        return;
    }
    mBrowser->setExp(ui->exp->value());
}

void SearchOptionsWidget::onLinesAfterValueChanged() {
    if (!mActive || !mBrowser) {
        return;
    }
    if (mBrowser->executed()) {
        emit clone();
        return;
    }
    mBrowser->setLinesAfter(ui->linesAfter->value());
}

void SearchOptionsWidget::onLinesBeforeValueChanged() {
    if (!mActive || !mBrowser) {
        return;
    }
    if (mBrowser->executed()) {
        emit clone();
        return;
    }
    mBrowser->setLinesBefore(ui->linesBefore->value());
}

void SearchOptionsWidget::onCacheFileListClicked(bool value) {
    if (!mActive || !mBrowser) {
        return;
    }
    if (mBrowser->executed()) {
        emit clone();
        return;
    }
    mBrowser->setCacheFileList(value);
}
