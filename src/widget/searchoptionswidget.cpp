#include "searchoptionswidget.h"
#include "ui_searchoptionswidget.h"

#include "searchbrowser.h"
#include "rxcollector.h"
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include "worker.h"
#include "anchorclickhandler.h"

#include <QAction>
#include "regexpreplacement.h"
#include <QCheckBox>

SearchOptionsWidget::SearchOptionsWidget(QWidget *parent) :
    QWidget(parent),
    /*mActive(true),
    mBrowser(0),
    mWorker(0),
    mClickHandler(0),
    mMode(ModeSearch),
    mCacheFileList(0),*/
    ui(new Ui::SearchOptionsWidget)
{
    ui->setupUi(this);
    ui->replace->setEnabled(false);

    connect(ui->pattern,SIGNAL(valueChanged(RegExp)),this,SIGNAL(patternChanged(RegExp)));
    connect(ui->filter,SIGNAL(valueChanged(RegExpPath)),this,SIGNAL(filterChanged(RegExpPath)));
    connect(ui->path,SIGNAL(textChanged(QString)),this,SIGNAL(pathChanged(QString)));
    connect(ui->replacement,SIGNAL(valueChanged(RegExpReplacement)),this,SIGNAL(replacementChanged(RegExpReplacement)));

    connect(ui->pattern,SIGNAL(returnPressed()),this,SIGNAL(search()));
    connect(ui->filter,SIGNAL(returnPressed()),this,SIGNAL(search()));

    connect(ui->search,SIGNAL(clicked()),this,SIGNAL(search()));
    connect(ui->preview,SIGNAL(clicked()),this,SIGNAL(preview()));
    connect(ui->replace,SIGNAL(clicked()),this,SIGNAL(replace()));

    connect(ui->replacement,SIGNAL(returnPressed()),this,SIGNAL(preview()));

    //connect(ui->notBinary,SIGNAL(stateChanged(bool)),this,SIGNAL(notBinaryChanged(bool)));

    hideFileCount();
    ui->path->checkBox()->setText("cache file list");

    QWidgetList widgets = this->widgets();
    for(int i=0;i<widgets.size()-1;i++) {
        QWidget::setTabOrder(widgets[i], widgets[i+1]);
    }

}

QWidgetList SearchOptionsWidget::widgets() {
    QWidgetList result;
    result << ui->path->widgets()
           << ui->select
           << ui->filter->widgets()
           << ui->pattern->widgets()
           << ui->search
           << ui->replacement->widgets()
           << ui->preview
           << ui->replace;
    return result;
}

SearchOptionsWidget::~SearchOptionsWidget()
{
    delete ui;
}

void SearchOptionsWidget::fixLayout() {
    QRect rect = ui->filter->matchCaseCheckBox()->geometry();
    ui->path->checkBox()->setFixedWidth(rect.width());
    QCheckBox* checkBox = ui->filter->notBinary();
    ui->gridLayout->addWidget(checkBox, 1, 2);
}

#if 0
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

#endif

void SearchOptionsWidget::setMode(Mode mode)
{
    mMode = mode;
    QWidgetList widgets;
    widgets << ui->replaceLabel << ui->replace << ui->preview << ui->replacement;
    foreach(QWidget* widget, widgets) {
        widget->setVisible(mode == Mode::Preview || mode == Mode::Replace);
    }
    ui->search->setVisible(mode == Mode::Search);
}

bool SearchOptionsWidget::renameFiles() const
{
    return ui->replacement->value().renameFiles();
}

#if 0

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


void SearchOptionsWidget::setActive(bool active)
{
    mActive = active;
}
#endif

void SearchOptionsWidget::on_select_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, QString(), ui->path->text());
    if (path.isEmpty()) {
        return;
    }
    ui->path->setText(QDir::toNativeSeparators(path));
}

#if 0
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
#endif

#if 0
void SearchOptionsWidget::setCacheFileList(QAction *action)
{
    ui->fileCount->setVisible(action->isChecked());
}
#endif

void SearchOptionsWidget::setReplaceEnabled(bool enabled)
{
    ui->replace->setEnabled(enabled);
}

void SearchOptionsWidget::setViewOptions(const ViewOptions &options)
{
    mViewOptions = options;
    ui->pattern->setChildVisible(options.search(), options.search());
    ui->filter->setVisible(options.filter());
    ui->filter->notBinary()->setVisible(options.filter());
    ui->filterLabel->setVisible(options.filter());
    ui->path->checkBox()->setVisible(options.cache());
}

ViewOptions SearchOptionsWidget::viewOptions() const {
    return mViewOptions;
}

void SearchOptionsWidget::showFileCount(int filtered, int total)
{
    if (filtered < 0) {
        ui->fileCount->setText("? / ?");
    } else {
        ui->fileCount->setText(QString("%1 / %2").arg(filtered).arg(total));
    }
    ui->fileCount->show();
}

void SearchOptionsWidget::hideFileCount()
{
    ui->fileCount->hide();
}

void SearchOptionsWidget::setPreviewEnabled(bool enabled) {
    ui->preview->setEnabled(enabled);
}

bool SearchOptionsWidget::cacheFileListChecked() const
{
    return ui->path->checkBox()->isChecked();
}

void SearchOptionsWidget::setCacheFileListChecked(bool value)
{
    ui->path->checkBox()->setChecked(value);
}

QCheckBox* SearchOptionsWidget::cacheFileList() {
    return ui->path->checkBox();
}

#if 0
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
#endif

void SearchOptionsWidget::collect(Mode mode)
{

    RXCollector* collector = RXCollector::instance();
    //mActive = false;

    if (mode == Mode::Search) {
        collector->collect(ui->pattern->value());
        collector->collect(ui->filter->value());
    } else if (mode == Mode::Replace){
        collector->collectReplacement(ui->replacement->value().pattern());
    }

    collector->collectPath(ui->path->text());

    //updateCollector();

}

QString SearchOptionsWidget::path() const
{
    return ui->path->text();
}

void SearchOptionsWidget::setPath(const QString &path)
{
    ui->path->setText(path);
}

void SearchOptionsWidget::loadCollected() {

    RXCollector* collector = RXCollector::instance();
    collector->load(ui->pattern);
    collector->load(ui->filter);
    collector->load(ui->replacement);
    collector->load(ui->path->lineEdit());
}

bool SearchOptionsWidget::validate()
{
    QPalette palette = this->palette();

    bool ok1 = ui->filter->validate(palette);
    bool ok2 = ui->pattern->validate(palette);
    //bool ok3 = mMode == ModeReplace ? ui->replacement->validate(palette) : true;
    bool ok3 = true;
    return ok1 && ok2 && ok3;
}

RegExpPath SearchOptionsWidget::filter() const
{
    return ui->filter->value();
}

void SearchOptionsWidget::setFiler(const RegExpPath& value)
{
    ui->filter->setValue(value);
}

/*
bool SearchOptionsWidget::notBinary() const {
    return ui->notBinary->isChecked();
}*/

void SearchOptionsWidget::setPattern(const RegExp& value)
{
    ui->pattern->setValue(value);
}

void SearchOptionsWidget::setReplacement(const RegExpReplacement& value) {
    ui->replacement->setValue(value);
}

QLineEdit *SearchOptionsWidget::pathEdit() const
{
    return ui->path->lineEdit();
}
