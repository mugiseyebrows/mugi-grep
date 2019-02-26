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
    mListenResultCurrentChanged(true),
    mSetValues(true)
{
    ui->setupUi(this);

    mParent = qobject_cast<QTabWidget*>(parent);

    mWorker = new Worker();
    mThread = new QThread();

    mWorker->moveToThread(mThread);

    connect(this,SIGNAL(search(int,QString,RegExpPath,bool,RegExp,int,int,bool)),
            mWorker,SLOT(onSearch(int,QString,RegExpPath,bool,RegExp,int,int,bool)));
    connect(mWorker,SIGNAL(found(int,QString,int,int,int,QString)),
            this,SLOT(onFound(int,QString,int,int,int,QString)));

    //connect(ui->options,SIGNAL())

    connect(this,SIGNAL(countMatchedFiles(QString,RegExpPath,bool)),mWorker,SLOT(onCountMatchedFiles(QString,RegExpPath,bool)));
    connect(mWorker,SIGNAL(count(int,int)),this,SLOT(onCountMatchedFiles(int,int)));

    connect(this,SIGNAL(searchMore(int)),mWorker,SLOT(onSearchMore(int)));

    connect(this,SIGNAL(finishSearch(int)),mWorker,SLOT(onFinishSearch(int)));

    connect(ui->options,SIGNAL(clone()),this,SLOT(onClone()));

    connect(ui->options,SIGNAL(search()),this,SLOT(onSearch()));

    connect(ui->options,SIGNAL(tabTitle(QString,bool)),this,SLOT(onTabTitle(QString,bool)));

    //connect(ui->searchFilter,SIGNAL(textChanged()),this,SLOT(onSearchFilterTextChanged()));
    //connect(ui->searchExp,SIGNAL(textChanged()),this,SLOT(onSearchExpTextChanged()));

    mThread->start();

    ui->progressBar->hide();
    ui->cancel->hide();

    while(ui->results->count() > 0) {
        ui->results->removeTab(0);
    }

    ui->status1->hide();
    ui->status2->setTextElideMode(Qt::ElideMiddle);
    ui->status2->hide();

    //ui->fileCount->setText(QString());

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
}

void SessionWidget::onSearch() {
    SearchBrowser* browser = currentTab();
    int searchId = SearchId::instance()->next();
    browser->setSearchId(searchId);
    ui->options->collect();
    ui->options->emitTabTitle();
}

void SessionWidget::onTabTitle(QString title, bool isExecuted) {
    if (!isExecuted) {
        title = title + "*";
    }
    ui->results->setTabText(ui->results->currentIndex(),title);
}

void SessionWidget::cancelAll()
{

}

QString SessionWidget::readPath() const
{
    //return ui->path->text();
}

void SessionWidget::serialize(QJsonObject& json) const
{
    json["path"] = ui->options->path();
}

void SessionWidget::deserialize(const QJsonObject &v)
{
    ui->options->setPath(v.value("path").toString());
}

void SessionWidget::startRead()
{
    //on_read_clicked();
}

void SessionWidget::updateCollector()
{
    //RXCollector::instance()->load(ui->searchFilter);
    //RXCollector::instance()->load(ui->searchExp);
    //qDebug() << "SessionWidget::updateCollector()";
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

SearchBrowser *SessionWidget::currentResult() const
{
    QWidget* w = ui->results->widget(ui->results->currentIndex());
    return qobject_cast<SearchBrowser*>(w);
}
#if 0
void SessionWidget::on_linesAfter_returnPressed() {
    on_search_clicked();
}

void SessionWidget::on_linesBefore_returnPressed() {
    on_search_clicked();
}


void SessionWidget::on_search_clicked() {

    SearchBrowser* browser = currentTab();
    if (!browser) {
        qDebug() << "!browser";
        return;
    }
    if (browser->executed()) {
        return;
    }

    int searchId = SearchId::instance()->next();
    browser->setSearchId(searchId);

    QString path = ui->options->path();
    RegExpPath filter = browser->filter();
    RegExp search_ = browser->exp();
    int linesBefore = browser->linesBefore();
    int linesAfter = browser->linesAfter();
    bool cacheFileList = browser->cacheFileList();
    bool notBinary = true;

    mCancel = false;

#if 0
    emit search(searchId,path,filter,notBinary,search_,linesBefore,linesAfter,cacheFileList);

    ui->status1->setText(QString("Building path list"));
    ui->status2->setText(QString());

    ui->status1->setVisible(true);
    ui->cancel->setVisible(true);
#endif


#if 0
    if (mResults.size() >= RESULT_TAB_LIMIT) {
        int index = ui->results->count() - 1;
        ui->results->widget(index)->deleteLater();
        ui->results->removeTab(index);
    }

    mCancel = false;

    QString path = ui->path->text();
    RegExpPath filter = ui->searchFilter->value();
    RegExp search_ = ui->searchExp->value();

    bool notBinary = ui->notBinary->isChecked();
    int linesBefore = ui->linesBefore->value();
    int linesAfter = ui->linesAfter->value();
    bool cacheFileList = ui->cacheFileList->isChecked();

    // add tab

    mSearchId = SearchId::instance()->next();

    SearchBrowser* browser = new SearchBrowser();
    //mResults[mSearchId] = browser;

    mClickHandler->connectBrowser(browser);

    QString title = ui->searchExp->value().include();

    //qDebug() << "title" << title << ui->searchExp->value().exps();

    int maxTitleChars = 20;
    if (title.size() > maxTitleChars) {
        title = title.mid(0,maxTitleChars-1) + "…";
    }

    ui->searchFilter->enableTextChanged(false);
    
    ui->results->insertTab(0,browser,title);
    ui->results->setCurrentIndex(0);

    ui->searchFilter->enableTextChanged(true);
    
    //qDebug() << "mResults[mSearchId] = browser" << mSearchId << browser;

    emit search(mSearchId,path,filter,notBinary,search_,linesBefore,linesAfter,cacheFileList);

    ui->status1->setText(QString("Building path list"));
    ui->status2->setText(QString());

    ui->searchFilter->enableTextChanged(false);

    RXCollector::instance()->collect(filter);
    RXCollector::instance()->collect(search_);
    updateCollector();

    ui->searchFilter->enableTextChanged(true);

    ui->status1->setVisible(true);


#endif

}

void SessionWidget::on_selectPath_clicked() {


    QString path = QFileDialog::getExistingDirectory(this, QString(), ui->path->text());
    if (path.isEmpty()) {
        return;
    }

    ui->path->setText(QDir::toNativeSeparators(path));

    //on_read_clicked();
}
#endif
#if 0
void SessionWidget::on_searchExp_returnPressed() {
    on_search_clicked();
}


void SessionWidget::on_searchFilter_returnPressed() {
    on_search_clicked();
}
#endif

void SessionWidget::on_cancel_clicked()
{
    /*for (int i=0;i<Worker::TaskTypeSize;i++)
        emit cancel(i);
    qDebug() << "on_cancel_clicked";*/

    mCancel = true;
}

void SessionWidget::on_saveText_clicked()
{
    if (!currentResult())
        return;
    QString path = QFileDialog::getSaveFileName(this,QString(),QString(),"Text (*.txt)");
    if (path.isEmpty())
        return;
    save(path,currentResult()->toPlainText());
}

void SessionWidget::on_saveHtml_clicked()
{
    if (!currentResult())
        return;
    QString path = QFileDialog::getSaveFileName(this,QString(),QString(),"Html (*.html)");
    if (path.isEmpty())
        return;
    save(path,currentResult()->toHtml());
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

    if (i >= 0) {
        ui->status1->setText(QString("Processed %1 files out of %2 (%3 filtered out)").arg(i).arg(t).arg(s));

        QString status2 = i == t ? QString() : QString("Last file: %1").arg(path);
        ui->status2->setText(status2);
        ui->status2->setVisible(!status2.isEmpty());

        ui->progressBar->setMaximum(t);
        ui->progressBar->setValue(i);
        ui->progressBar->setVisible( i != t );
        //qDebug() << i << t << s;
    }
    ui->cancel->setVisible( i != t );

    //emit searchMore(id);

    if (i < t && !mCancel) {
        emit searchMore(searchId);
    } else {
        emit finishSearch(searchId);
    }

    if (mCancel) {
        ui->progressBar->setVisible(false);
        ui->cancel->setVisible(false);
        ui->status1->setText(QString("Search aborted"));
        ui->status2->setText(QString());
        ui->status2->hide();
    }
}

void SessionWidget::onCountMatchedFiles(int matched, int total)
{
    //ui->fileCount->setText(QString("%1 / %2 files").arg(matched).arg(total));
}

SearchBrowser* SessionWidget::currentTab() {
    return tab(ui->results->currentIndex());
}

SearchBrowser* SessionWidget::tab(int index) {
    return qobject_cast<SearchBrowser*>(ui->results->widget(index));
}

void SessionWidget::on_results_currentChanged(int index) {

    if (!mListenResultCurrentChanged) {
        return;
    }

    qDebug() << "on_results_currentChanged" << index;

    SearchBrowser* browser = tab(index);
    if (!browser) {
        qDebug() << "not a browser at index" << index;
        return;
    }

    ui->options->setBrowser(browser,mSetValues);

    mSetValues = true;

    /*ui->searchFilter->enableTextChanged(false);

    ui->searchFilter->setValue(b->filter());
    ui->searchExp->setValue(b->exp());
    ui->linesBefore->setValue(b->linesBefore());
    ui->linesAfter->setValue(b->linesAfter());
    ui->cacheFileList->setChecked(b->cacheFileList());

    ui->searchFilter->enableTextChanged(true);*/

}

#if 0
void SessionWidget::on_path_textChanged(const QString &path)
{
    if (!mParent) {
        qDebug() << "cannot cast";
        return;
    }
    mParent->setTabText(mParent->indexOf(this),QFileInfo(path).fileName());
}
#endif

void SessionWidget::countMatchedFiles() {
#if 0
    qDebug() << "onSearchFilterTextChanged";
    if (!ui->cacheFileList->isChecked()) {
        return;
    }
    QString path = ui->path->text();
    RegExpPath filter = ui->searchFilter->value();
    bool notBinary = ui->notBinary->isChecked();
    emit countMatchedFiles(path,filter,notBinary);
    ui->fileCount->setText("? / ? files");
#endif
}

#if 0
SearchBrowser* SessionWidget::createTab(const QString& name, SearchBrowser* browser, bool append, bool select) {

    if (select) {
        mListenResultCurrentChanged = false;
    }
    SearchBrowser* browser_ = new SearchBrowser();
    if (browser) {
        browser->copy(browser_);
    }
    mClickHandler->connectBrowser(browser_);
    if (append) {
        ui->results->addTab(browser_,name);
    }
    if (select) {
        ui->results->setCurrentWidget(browser_);
        mListenResultCurrentChanged = true;
    }
    return browser_;


}

void SessionWidget::onSearchFilterTextChanged() {

    countMatchedFiles();
    SearchBrowser* browser = currentTab();
    if (browser->executed()) {
        SearchBrowser* browser_ = createTab(QString::number(ui->results->count()+1), browser);
        browser_->setFilter(ui->searchFilter->value());
        return;
    }
    browser->setFilter(ui->searchFilter->value());
}

void SessionWidget::onSearchExpTextChanged() {

    SearchBrowser* browser = currentTab();
    if (browser->executed()) {
        SearchBrowser* browser_ = createTab(QString::number(ui->results->count()+1), browser);
        browser_->setExp(ui->searchExp->value());
        return;
    }
    browser->setExp(ui->searchExp->value());

}

void SessionWidget::on_selectFiles_clicked()
{

    QString path = ui->path->text();

    SelectFilesDialog dialog(path, ui->searchFilter->value(), mWorker, mClickHandler, this);

    if (dialog.exec() == QDialog::Accepted) {
        ui->searchFilter->setValue(dialog.filter());
    }


}
#endif
