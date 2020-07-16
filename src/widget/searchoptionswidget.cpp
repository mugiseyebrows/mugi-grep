#include "searchoptionswidget.h"
#include "ui_searchoptionswidget.h"

#include "searchbrowser.h"
#include "rxcollector.h"
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include "worker.h"
#include "anchorclickhandler.h"
#include "selectfilesdialog.h"
#include <QAction>

SearchOptionsWidget::SearchOptionsWidget(QWidget *parent) :
    QWidget(parent),
    mActive(true),
    mBrowser(0),
    mWorker(0),
    mClickHandler(0),
    mMode(ModeSearch),
    mCacheFileList(0),
    ui(new Ui::SearchOptionsWidget)
{
    ui->setupUi(this);
    ui->search->setEnabled(false);
}

SearchOptionsWidget::~SearchOptionsWidget()
{
    delete ui;
}

void SearchOptionsWidget::setBrowser(SearchBrowser *browser, bool setValues)
{
    mBrowser = browser;
    if (setValues) {
        //updateCompletions();
        mActive = false;
        ui->filter->setValue(browser->filter());
        ui->exp->setValue(browser->exp());
        ui->linesBefore->setValue(browser->linesBefore());
        ui->linesAfter->setValue(browser->linesAfter());
        ui->notBinary->setChecked(browser->notBinary());
        ui->replacement->setValue(browser->replacement());
        ui->replacement->setPreserveCase(browser->preserveCase());
        ui->showFileName->setChecked(browser->showFileName());
        ui->showLineNumber->setChecked(browser->showLineNumber());
        ui->onlyMatched->setChecked(browser->onlyMatched());
        ui->filter->hide();
        ui->filter->show(); // force layout to recalculate
    }

    mActive = true;
}

void SearchOptionsWidget::setMode(SearchOptionsWidget::Mode mode)
{
    mMode = mode;
    QWidgetList widgets;
    widgets << ui->replaceLabel << ui->replace << ui->preview << ui->replacement;
    foreach(QWidget* widget, widgets) {
        widget->setVisible(mode == ModeReplace);
    }
    ui->search->setVisible(mode == ModeSearch);
}


void SearchOptionsWidget::init(Worker *worker, AnchorClickHandler* clickHandler) {

    connect(ui->filter,SIGNAL(returnPressed()),this,SIGNAL(search()));
    connect(ui->exp,SIGNAL(returnPressed()),this,SIGNAL(search()));
    connect(ui->filter,SIGNAL(textChanged()),this,SLOT(onFilterTextChanged()));
    connect(ui->exp,SIGNAL(textChanged()),this,SLOT(onExpTextChanged()));
    connect(ui->linesAfter,SIGNAL(valueChanged(int)),this,SLOT(onLinesAfterValueChanged()));
    connect(ui->linesBefore,SIGNAL(valueChanged(int)),this,SLOT(onLinesBeforeValueChanged()));
    connect(ui->notBinary,SIGNAL(clicked(bool)),this,SLOT(onNotBinaryClicked(bool)));
    connect(ui->filter,SIGNAL(caseClicked(bool)),this,SLOT(onFilterTextChanged()));
    connect(ui->exp,SIGNAL(caseClicked(bool)),this,SLOT(onExpTextChanged()));
    connect(ui->linesBefore,SIGNAL(returnPressed()),this,SIGNAL(search()));
    connect(ui->linesAfter,SIGNAL(returnPressed()),this,SIGNAL(search()));
    connect(ui->replacement,SIGNAL(returnPressed()),this,SIGNAL(preview()));
    connect(ui->replacement,SIGNAL(textChanged(QString)),this,SLOT(onReplacementTextChanged(QString)));
    connect(ui->replacement,SIGNAL(preserveCaseClicked(bool)),this,SLOT(onPreserveCaseClicked(bool)));

    mWorker = worker;
    mClickHandler = clickHandler;
    connect(this,SIGNAL(countMatchedFiles(QString,RegExpPath,bool)),
            mWorker,SLOT(onCountMatchedFiles(QString,RegExpPath,bool)));
    connect(mWorker,SIGNAL(count(int,int)),this,SLOT(onCountMatchedFiles(int,int)));

    ui->fileCount->setText("? / ?");
    setMode(mMode);
}


QString SearchOptionsWidget::path() const
{
    return ui->path->text();
}

void SearchOptionsWidget::setPath(const QString &path)
{
    ui->path->setText(path);
}

bool SearchOptionsWidget::validate()
{
    QPalette palette = this->palette();

    bool ok1 = ui->filter->validate(palette);
    bool ok2 = ui->exp->validate(palette);
    bool ok3 = mMode == ModeReplace ? ui->replacement->validate(palette) : true;
    return ok1 && ok2 && ok3;
}

void SearchOptionsWidget::setBrowserValues()
{
    mBrowser->setFilter(ui->filter->value());
    mBrowser->setExp(ui->exp->value());
    mBrowser->setLinesBefore(ui->linesBefore->value());
    mBrowser->setLinesAfter(ui->linesAfter->value());
    mBrowser->setNotBinary(ui->notBinary->isChecked());
    mBrowser->setReplacement(ui->replacement->value());
    mBrowser->setPreserveCase(ui->replacement->preserveCase());
    mBrowser->setShowFileName(ui->showFileName->isChecked());
    mBrowser->setShowLineNumber(ui->showLineNumber->isChecked());
    mBrowser->setOnlyMatched(ui->onlyMatched->isChecked());
}

void SearchOptionsWidget::collect()
{
    RXCollector* collector = RXCollector::instance();
    //mActive = false;
    collector->collect(ui->exp->value());
    collector->collect(ui->filter->value());
    collector->collect(ui->replacement->value());
    //updateCollector();
}

void SearchOptionsWidget::updateCompletions() {
    mActive = false;
    RXCollector* collector = RXCollector::instance();
    collector->load(ui->exp);
    collector->load(ui->filter);
    collector->load(ui->replacement);
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

void SearchOptionsWidget::countMatchedFiles() {
    // todo cache

    if (!mCacheFileList) {
        //qDebug() << "!mCacheFileList";
        return;
    }

    if (!mCacheFileList->isChecked()) {
        //ui->fileCount->setText(QString());
        return;
    }
    ui->fileCount->setText("? / ?");
    QString path = ui->path->text();
    RegExpPath filter = ui->filter->value();
    bool notBinary = ui->notBinary->isChecked();
    emit countMatchedFiles(path,filter,notBinary);
}

void SearchOptionsWidget::setCanReplace(bool can)
{
    ui->replace->setEnabled(can);
}

void SearchOptionsWidget::onFilterTextChanged() {
    if (!mActive || !mBrowser) {
        return;
    }
    if (mBrowser->isExecuted()) {
        emit clone();
        return;
    }
    countMatchedFiles();
    mBrowser->setFilter(ui->filter->value());
}

void SearchOptionsWidget::onExpTextChanged() {
    ui->search->setEnabled(!ui->exp->value().isEmpty());
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

    QString include = ui->exp->value().include();
    QString exclude = ui->exp->value().exclude();
    QString title;
    if (include.isEmpty() && exclude.isEmpty()) {
        title = "";
    } else if (!include.isEmpty()) {
        title = include;
    } else {
        title = "~" + exclude;
    }
    emit tabTitle(title, mBrowser->isExecuted());
}

void SearchOptionsWidget::onLinesAfterValueChanged() {
    /*if (!mActive || !mBrowser) {
        return;
    }
    if (mBrowser->isExecuted()) {
        emit clone();
        return;
    }*/
    mBrowser->setLinesAfter(ui->linesAfter->value());
}

void SearchOptionsWidget::onLinesBeforeValueChanged() {
    /*if (!mActive || !mBrowser) {
        return;
    }
    if (mBrowser->isExecuted()) {
        emit clone();
        return;
    }*/
    mBrowser->setLinesBefore(ui->linesBefore->value());
}

void SearchOptionsWidget::select() {
    QString path = ui->path->text();
    SelectFilesDialog dialog(path, ui->filter->value(), mWorker, mClickHandler, this);
    if (dialog.exec() == QDialog::Accepted) {
        ui->filter->setValue(dialog.filter());
    }
}

void SearchOptionsWidget::setCacheFileList(QAction *action)
{
    mCacheFileList = action;
    ui->fileCount->setVisible(action->isChecked());
    connect(action,SIGNAL(toggled(bool)),this,SLOT(onCacheToggled(bool)));
}

void SearchOptionsWidget::onCacheToggled(bool checked) {
    ui->fileCount->setVisible(checked);
    if (checked && !ui->filter->value().isEmpty()) {
        emit countMatchedFiles();
    }
}

void SearchOptionsWidget::onCountMatchedFiles(int matched, int total)
{
    ui->fileCount->setText(QString("%1 / %2 files").arg(matched).arg(total));
}

void SearchOptionsWidget::on_path_textChanged(QString path)
{
    emit pathChanged(path);
}

void SearchOptionsWidget::onNotBinaryClicked(bool value) {
    /*if (!mActive || !mBrowser) {
        return;
    }
    if (mBrowser->isExecuted()) {
        emit clone();
        return;
    }*/
    mBrowser->setNotBinary(value);
}

void SearchOptionsWidget::on_replace_clicked()
{
    emit replace();
}

void SearchOptionsWidget::on_preview_clicked()
{
    emit preview();
}

void SearchOptionsWidget::onReplacementTextChanged(QString value)
{
    if (!mActive || !mBrowser) {
        return;
    }
    if (mBrowser->isExecuted()) {
        emit clone();
        return;
    }
    mBrowser->setReplacement(value);
}

void SearchOptionsWidget::onPreserveCaseClicked(bool value) {
    mBrowser->setPreserveCase(value);
}

void SearchOptionsWidget::on_showFileName_clicked(bool checked)
{
    mBrowser->setShowFileName(checked);
}

void SearchOptionsWidget::on_showLineNumber_clicked(bool checked)
{
    mBrowser->setShowLineNumber(checked);
}

void SearchOptionsWidget::on_onlyMatched_clicked(bool checked)
{
    mBrowser->setOnlyMatched(checked);
}
