#include "widget/sessionwidget.h"
#include "ui_sessionwidget.h"

#include <QDirIterator>
#include <QDebug>
#include <QFileDialog>
#include <QTextCodec>
#include <QProgressDialog>
#include <QThread>
#include <QDesktopServices>
#include <QProcess>
#include <QTabWidget>
#include <QUrlQuery>
#include <QMessageBox>

#include "widget/searchbrowser.h"
#include "utils/sl.h"

#include "worker.h"
#include "rxcollector.h"
#include "searchid.h"
#include "settings.h"
#include <QMessageBox>
#include "anchorclickhandler.h"
#include "widget/selectfilesdialog.h"

#define RESULT_TAB_LIMIT 10

SessionWidget::SessionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SessionWidget),
    mClickHandler(new AnchorClickHandler()),
    //mListenResultCurrentChanged(true),
    mSetValues(true)
{
    ui->setupUi(this);

    mTabWidget = qobject_cast<QTabWidget*>(parent);

    mWorker = new Worker();
    mThread = new QThread();

    mWorker->moveToThread(mThread);

    connect(this,SIGNAL(search(int,QString,RegExpPath,bool,RegExp,int,int,bool)),
            mWorker,SLOT(onSearch(int,QString,RegExpPath,bool,RegExp,int,int,bool)));
    connect(mWorker,SIGNAL(found(int,QString,int,int,int,QString)),
            this,SLOT(onFound(int,QString,int,int,int,QString)));
    connect(this,SIGNAL(searchMore(int)),mWorker,SLOT(onSearchMore(int)));
    connect(this,SIGNAL(finishSearch(int)),mWorker,SLOT(onFinishSearch(int)));
    connect(ui->options,SIGNAL(clone()),this,SLOT(onClone()));
    connect(ui->options,SIGNAL(search()),this,SLOT(onSearch()));
    connect(ui->options,SIGNAL(tabTitle(QString,bool)),this,SLOT(onTabTitle(QString,bool)));
    connect(ui->options,SIGNAL(pathChanged(QString)),this,SLOT(onPathChanged(QString)));

    connect(ui->progress,SIGNAL(canceled()),this,SLOT(onCanceled()));

    mThread->start();

    while(ui->results->count() > 0) {
        ui->results->removeTab(0);
    }

    ui->options->init(mWorker, mClickHandler);
    ui->progress->init();

    SearchBrowser* browser_ = new SearchBrowser();
    mClickHandler->connectBrowser(browser_);
    ui->results->addTab(browser_,QString());
}

SessionWidget::~SessionWidget()
{
    mCancel = true;
    if (mThread) {
        mThread->quit();
        mThread->wait();
    }
    delete mThread;
    mThread = nullptr;
    delete ui;
}

void SessionWidget::onClone() {
    SearchBrowser* browser = currentTab();
    SearchBrowser* browser_ = new SearchBrowser();
    browser->copy(browser_);
    mClickHandler->connectBrowser(browser_);
    ui->results->addTab(browser_,browser->exp().include());
    mSetValues = false;
    ui->results->setCurrentWidget(browser_);
    ui->options->setBrowserValues();
    ui->options->emitTabTitle();

    if (ui->results->count() >= RESULT_TAB_LIMIT) {
        int index = oldestTabIndex();
        if (index > -1) {
            QWidget* widget = ui->results->widget(index);
            ui->results->removeTab(index);
            widget->deleteLater();
        }
    }
}

void SessionWidget::onSearch() {

    SearchBrowser* browser = currentTab();
    if (browser->isExecuted()) {
        return;
    }
    mCancel = false;
    int searchId = SearchId::instance()->next();
    browser->setSearchId(searchId);
    ui->options->collect();
    ui->options->emitTabTitle();

    emit search(searchId,ui->options->path(),browser->filter(),browser->notBinary(),browser->exp(),
                browser->linesBefore(),browser->linesAfter(),browser->cacheFileList());

    ui->progress->started();
}

int SessionWidget::oldestTabIndex() { // todo test me
    int count = ui->results->count();
    if (count < 1) {
        return -1;
    }
    QDateTime dateTime = tab(0)->changed();
    int index = 0;
    for(int i=1;i<count;i++) {
        SearchBrowser* browser = tab(i);
        QDateTime dateTime_ = browser->changed();
        if (dateTime > dateTime_) {
            dateTime = dateTime_;
            index = i;
        }
    }
    return index;
}

void SessionWidget::onTabTitle(QString title, bool isExecuted) {
    if (!isExecuted) {
        title = title + "*";
    }
    ui->results->setTabText(ui->results->currentIndex(),title);
}

QString SessionWidget::path() const
{
    return ui->options->path();
}

void SessionWidget::serialize(QJsonObject& json) const
{
    json["path"] = ui->options->path();
}

void SessionWidget::deserialize(const QJsonObject &v)
{
    ui->options->setPath(v.value("path").toString());
}

void SessionWidget::updateCollector()
{
    ui->options->updateCollector();
}

void SessionWidget::save(const QString &path, const QString &text)
{
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForName("UTF-8"));
    stream << text;
    stream.flush();
    file.close();
}

void SessionWidget::onCanceled() {
    mCancel = true;
}

void SessionWidget::on_saveText_clicked()
{
    save(true);
}

void SessionWidget::on_saveHtml_clicked()
{
    save(false);
}

void SessionWidget::save(bool plain) {
    SearchBrowser* browser = currentTab();
    if (!browser) {
        return;
    }
    QString filter = plain ? "Text (*.txt)" : "Html (*.html)";
    QString path = QFileDialog::getSaveFileName(this,QString(),QString(),filter);
    if (path.isEmpty()) {
        return;
    }
    save(path,plain ? browser->toPlainText() : browser->toHtml());
}

SearchBrowser* SessionWidget::find(int searchId) {
    int count = ui->results->count();
    for(int i=0;i<count;i++) {
        SearchBrowser* browser = tab(i);
        if (browser->searchId() == searchId) {
            return browser;
        }
    }
    return 0;
}

void SessionWidget::onFound(int searchId, QString res, int i, int t, int s, QString path)
{

    qDebug() << searchId << res.size() << "chars" << i << t << s << path;

    SearchBrowser* browser = find(searchId);

    if (!browser) {
        qDebug() << "onFound error no browser";
        return;
    }

    if (!res.isEmpty()) {
        browser->append(res);
    }

    ui->progress->progress(i,t,s,path);

    if (i < t && !mCancel) {
        emit searchMore(searchId);
    } else {
        emit finishSearch(searchId);
    }

    if (mCancel) {
        ui->progress->aborted();
    }
}

SearchBrowser* SessionWidget::currentTab() {
    return tab(ui->results->currentIndex());
}

SearchBrowser* SessionWidget::tab(int index) {
    return qobject_cast<SearchBrowser*>(ui->results->widget(index));
}

void SessionWidget::on_results_currentChanged(int index) {

    /*if (!mListenResultCurrentChanged) {
        return;
    }*/
    qDebug() << "on_results_currentChanged" << index;
    SearchBrowser* browser = tab(index);
    if (!browser) {
        qDebug() << "not a browser at index" << index;
        return;
    }
    ui->options->setBrowser(browser,mSetValues);
    ui->options->countMatchedFiles();
    mSetValues = true;
}

void SessionWidget::onPathChanged(QString path)
{
    if (!mTabWidget) {
        qDebug() << "cannot cast";
        return;
    }
    mTabWidget->setTabText(mTabWidget->indexOf(this),QFileInfo(path).fileName());
}

