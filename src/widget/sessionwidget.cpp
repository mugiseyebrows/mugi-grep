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
#include <QAction>
#include <QCheckBox>

#include "widget/searchbrowser.h"
#include "mode.h"

#include "worker.h"

#include "searchid.h"
#include "settings.h"
#include <QMessageBox>
#include "anchorclickhandler.h"

#include "widget/mainwindow.h"

#include <QCompleter>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QTreeView>
#include "searchtab.h"
#include "fileio.h"
#include "callonce.h"
#include "countfilesmanager.h"

#include "rxcollector.h"
#include "searchnamehits.h"
#include "completerhelper.h"

#define RESULT_TAB_LIMIT 10

SessionWidget::SessionWidget(Settings *settings, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SessionWidget),
    mClickHandler(new AnchorClickHandler(settings)),
    mListenOptions(false),
    mSettings(settings),
    /*mReplacementChanged(new CallOnce("mReplacementChanged", 500, this)),*/
    mGetListing(new CallOnce("mGetAllFiles", 500, this)),
    mCountFilesManager(new CountFilesManager(this))
{
    ui->setupUi(this);

    mTabWidget = qobject_cast<QTabWidget*>(parent);

    mWorker = new Worker();
    mThread = new QThread();

    mWorker->moveToThread(mThread);

    connect(this,SIGNAL(search(SearchParams)),mWorker,SLOT(onSearch(SearchParams)));
    connect(this,SIGNAL(searchMore(int)),mWorker,SLOT(onSearchMore(int)));
    connect(mWorker,SIGNAL(found(int,SearchHits,SearchNameHits)),this,SLOT(onFound(int,SearchHits,SearchNameHits)));

    connect(ui->options,SIGNAL(patternChanged(RegExp)),this,SLOT(onPatternChanged(RegExp)));
    connect(ui->options,SIGNAL(filterChanged(RegExpPath)),this,SLOT(onFilterChanged(RegExpPath)));
    connect(ui->options,SIGNAL(replacementChanged(RegExpReplacement)),this,SLOT(onReplacementChanged(RegExpReplacement)));
    connect(ui->options,SIGNAL(pathChanged(QString)),this,SLOT(onPathChanged(QString)));
    connect(ui->options,SIGNAL(search()),this,SLOT(onSearch()));

    //connect(this,SIGNAL(countFiles(CountFilesParams)),mWorker,SLOT(onCountFiles(CountFilesParams)));
    //connect(mWorker,SIGNAL(filesCounted(CountFilesParams)),this,SLOT(onFilesCounted(CountFilesParams)));

    connect(ui->options,SIGNAL(preview()),this,SLOT(onPreview()));
    connect(ui->options,SIGNAL(replace()),this,SLOT(onReplace()));
    connect(ui->progress,SIGNAL(canceled()),this,SLOT(onCancel()));

    connect(this,SIGNAL(replace(ReplaceParams)),mWorker,SLOT(onReplace(ReplaceParams)));
    connect(mWorker,SIGNAL(replaced(ReplacedParams)),this,SLOT(onReplaced(ReplacedParams)));

    //connect(mReplacementChanged,SIGNAL(call()),this,SLOT(onPreview()));
    //connect(mCountFiles,SIGNAL(call()),this,SLOT(onCountFiles()));
    connect(mGetListing,SIGNAL(call()),this,SLOT(onGetListing()));
    connect(this,SIGNAL(getListing(GetListingParams)),mWorker,SLOT(onGetListing(GetListingParams)));
    connect(mWorker,SIGNAL(listing(QString,QStringList)),this,SLOT(onListing(QString,QStringList)));

    connect(mCountFilesManager,SIGNAL(filesCounted()),this,SLOT(onFilesCounted()));
    connect(mCountFilesManager,SIGNAL(countFiles(CountFilesParams)),mWorker,SLOT(onCountFiles(CountFilesParams)));
    connect(mWorker,SIGNAL(filesCounted(CountFilesParams)),mCountFilesManager,SLOT(onFilesCounted(CountFilesParams)));

    connect(ui->options->cacheFileList(), SIGNAL(clicked(bool)), this, SLOT(onCacheFileListClicked(bool)));
    connect(mWorker,SIGNAL(renamed(int,int)),this,SLOT(onRenamed(int,int)));

    connect(this,SIGNAL(rename(RenameParams)),mWorker,SLOT(onRename(RenameParams)));

    mThread->start();

    while(ui->results->count() > 0) {
        ui->results->removeTab(0);
    }

    //ui->options->init(mWorker, mClickHandler);
    ui->progress->init();

    /*SearchBrowser* browser_ = new SearchBrowser();
    mClickHandler->connectBrowser(browser_);
    ui->results->addTab(browser_,QString());*/
    //connect(ui->options,SIGNAL())

    ui->progressGroup->hide();

    ui->options->setMode(Mode::Search);

    SearchTab* tab = createTab();
    ui->results->addTab(tab,"");
    mListenOptions = true;

    //tab->textBrowser()->setText("test");

    //RXCollector::instance()->load(ui->options->pathEdit());
}

void SessionWidget::onCacheFileListClicked(bool cacheFileList) {

    if (cacheFileList) {
        onFilesCounted();
    } else {
        ui->options->hideFileCount();
    }
}

SearchTab* SessionWidget::createTab() {
    SearchTab* tab = new SearchTab();
    mClickHandler->connectBrowser(tab->textBrowser());
    return tab;
}

void SessionWidget::copyToNewTab() {
    SearchTab* tab = this->currentTab();
    SearchTab* newTab = createTab();
    newTab->setParams(tab->params());
    newTab->setHits(tab->hits());
    newTab->setNameHits(tab->nameHits());
    newTab->setMode(tab->mode());
    newTab->setDisplayOptions(tab->displayOptions());
    newTab->trigRerender();
    QString title = ui->results->tabText(ui->results->currentIndex());
    ui->results->addTab(newTab, title);
    tab->params().setId(-1);
    tab->setHits(SearchHits());
    if (tab->mode() == Mode::Replace) {
        tab->setMode(Mode::Preview);
    }
    tab->trigRerender();
    updateTabText(ui->results->currentIndex());
    updateReplaceButton();

    int count = ui->results->count();

    if (count > RESULT_TAB_LIMIT) {
        QDateTime minUpdated = this->tab(0)->updated();
        int minIndex = 0;
        for(int i=1;i<count;i++) {
            QDateTime updated = this->tab(i)->updated();
            if (updated < minUpdated) {
                minUpdated = updated;
                minIndex = i;
            }
        }
        mQueedToRemove = minIndex;
    }

}

void SessionWidget::onPatternChanged(RegExp value) {
    if (!mListenOptions) {
        return;
    }
    SearchTab* tab = this->currentTab();
    if (!tab) {
        return;
    }
    if (tab->params().id() > -1) {
        copyToNewTab();
    }
    tab->params().setPattern(value);
    updateTabText(ui->results->currentIndex());
}

void SessionWidget::onFilterChanged(RegExpPath value) {
    if (!mListenOptions) {
        return;
    }
    SearchTab* tab = this->currentTab();
    if (!tab) {
        return;
    }
    if (tab->params().id() > -1) {
        copyToNewTab();
    }
    tab->params().setFilter(value);

    if (ui->options->cacheFileListChecked()) {
        onFilesCounted();
    }
}

void SessionWidget::onFilesCounted() {
    QString path = ui->options->path();
    RegExpPath filter = ui->options->filter();
    //bool notBinary = ui->options->notBinary();
    QPair<int,int> count = mCountFilesManager->count(path, filter);
    ui->options->showFileCount(count.first, count.second);
}

void SessionWidget::onCountFiles() {
    if (!ui->options->cacheFileListChecked()) {
        ui->options->hideFileCount();
        return;
    }
    onFilesCounted();
}

void SessionWidget::on_open_textChanged(QString)
{
    mGetListing->onPost();
}

void SessionWidget::onReplacementChanged(RegExpReplacement value) {
    if (!mListenOptions) {
        return;
    }
    SearchTab* tab = this->currentTab();
    if (!tab) {
        return;
    }
    tab->params().setReplacement(value);
    /*if (tab->mode() == Mode::Preview && !tab->hits().isEmpty()) {
        mReplacementChanged->onPost();
    }*/
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

#if 0
void SessionWidget::setCacheFileList(QAction *action)
{
    mCacheFileList = action;
    ui->options->setCacheFileList(action);
}
#endif

#if 0
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
#endif

SearchOptionsWidget* SessionWidget::options() const{
    return ui->options;
}

void SessionWidget::setMode(Mode mode)
{
    ui->options->setMode(mode);
    SearchTab* tab = currentTab();
    if (!tab) {
        return;
    }
    tab->setMode(mode);
    updateReplaceButton();
}

void SessionWidget::updateReplaceButton()
{
    SearchTab* tab = currentTab();
    bool enabled = tab && !tab->hits().isEmpty() && tab->mode() == Mode::Preview;
    ui->options->setReplaceEnabled(enabled);
    ui->options->setPreviewEnabled(tab->mode() == Mode::Preview);
}

void SessionWidget::select()
{
    //ui->options->select();
}



void SessionWidget::searchOrReplace(Worker::Action action) {
#if 0
    SearchBrowser* browser = currentTab();
    if (browser->exp().isEmpty()) {
        return;
    }

    bool valid = ui->options->validate();
    if (!valid) {
        return;
    }

    mCancel = false;
    emit collect();
    int searchId = SearchId::instance()->next();
    browser->setText(QString());
    browser->setSearchId(searchId);
    ui->options->emitTabTitle();

    SearchParams params = browser->params(action, searchId, ui->options->path(), mCacheFileList->isChecked());

    emit search(params);
    ui->progress->started();
#endif
}

void SessionWidget::onSearch() {

    SearchTab* tab = currentTab();
    if (tab->params().pattern().isEmpty()) {
        return;
    }

    bool valid = ui->options->validate();
    if (!valid) {
        return;
    }

    ui->options->collect(Mode::Search);

    mCancel = false;
    //emit collect();
    int searchId = SearchId::instance()->next();

    tab->params().setId(searchId);
    tab->params().setPath(ui->options->path());
    tab->params().setCacheFileList(ui->options->cacheFileListChecked());
    tab->hits().clear();
    tab->nameHits().clear();
    tab->trigRerender();

    updateTabText(ui->results->currentIndex());

    emit search(tab->params());
    ui->progressGroup->show();
    ui->progress->started();

    if (mQueedToRemove > -1) {
        ui->results->removeTab(mQueedToRemove);
        mQueedToRemove = -1;
    }

}

void SessionWidget::updateTabText(int index) {

    SearchTab* tab = this->tab(index);
    if (!tab) {
        return;
    }
    ui->results->setTabText(index, tabTitle(tab->params().pattern().include(), tab->params().id() > -1));
}

void SessionWidget::onPreview() {
    SearchTab* tab = currentTab();
    if (!tab) {
        return;
    }
    if (tab->mode() == Mode::Replace) {
        // already replaced
        return;
    }
    tab->setMode(Mode::Preview);
    if (tab->params().id() < 0) {
        if (tab->params().pattern().isEmpty()) {
            return;
        }
        onSearch();
    } else {
        tab->trigRerender();
    }
}

void SessionWidget::onReplace() {
    SearchTab* tab = currentTab();
    if (!tab) {
        return;
    }
    bool renameFiles = ui->options->renameFiles();
    ReplaceParams params = tab->replaceParams(renameFiles);
    ui->options->collect(Mode::Replace);
    emit replace(params);
}

void SessionWidget::onCancel() {
    SearchTab* tab = currentTab();
    if (!tab) {
        return;
    }
    int id = tab->params().id();
    if (id > -1) {
        mCanceled.insert(id);
    }
}

int SessionWidget::oldestTabIndex() { // todo test me
#if 0
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
#endif
    return -1;
}

QString SessionWidget::tabTitle(QString title, bool isExecuted) const {
    if (!isExecuted) {
        title = title + "*";
    }
    QFontMetrics fm(font());
    title = fm.elidedText(title,Qt::ElideMiddle,200);
    return title;
}

QString SessionWidget::path() const
{
    return ui->options->path();
}

QJsonValue SessionWidget::serialize() const
{
    QJsonObject obj;
    obj["path"] = ui->options->path();
    obj["cacheFileList"] = ui->options->cacheFileListChecked();
    return obj;
}

void SessionWidget::deserialize(const QJsonValue &v)
{
    QJsonObject obj = v.toObject();
    QString path = obj["path"].toString();
    bool cacheFileList = obj["cacheFileList"].toBool();
    ui->options->setPath(path);
    ui->options->setCacheFileListChecked(cacheFileList);
}

void SessionWidget::loadCollected()
{
    mListenOptions = false;
    ui->options->loadCollected();
    mListenOptions = true;
}

void SessionWidget::onCanceled() {
    mCancel = true;
}

void SessionWidget::save(Format format) {

    SearchTab* tab = currentTab();
    if (!tab) {
        return;
    }

    QString filter = format == Format::Text ? "Text (*.txt)" : "Html (*.html)";
    QString path = QFileDialog::getSaveFileName(this,QString(),QString(),filter);
    if (path.isEmpty()) {
        return;
    }

    QString text = format == Format::Text ? tab->toPlainText() : tab->toHtml();

    FileIO::writeLines(path, text);
}

SearchTab* SessionWidget::find(int searchId) {
    int count = ui->results->count();
    for(int i=0;i<count;i++) {
        SearchTab* tab = this->tab(i);
        //qDebug() << "tabid" << tab->params().id();
        if (tab->params().id() == searchId) {
            return tab;
        }
    }
    return 0;
}

SearchTab* SessionWidget::currentTab() {
    return tab(ui->results->currentIndex());
}

SearchTab* SessionWidget::tab(int index) {
    return qobject_cast<SearchTab*>(ui->results->widget(index));
}

void SessionWidget::on_results_currentChanged(int index) {

    if (index < 0) {
        return;
    }
    SearchTab* tab = this->tab(index);
    if (!tab) {
        return;
    }
    mListenOptions = false;
    ui->options->setPattern(tab->params().pattern());
    ui->options->setFiler(tab->params().filter());
    ui->options->setReplacement(tab->params().replacement());
    ui->options->setMode(tab->mode());

    if (ui->options->cacheFileListChecked()) {
        onFilesCounted();
    }

    mListenOptions = true;

    updateReplaceButton();

#if 0
    /*if (!mListenResultCurrentChanged) {
        return;
    }*/
    //qDebug() << "on_results_currentChanged" << index << this;
    SearchTab* browser = tab(index);
    if (!browser) {
        //qDebug() << "not a browser at index" << index;
        return;
    }
    ui->options->setBrowser(browser,mSetValues);
    //ui->options->countMatchedFiles();
    mSetValues = true;

    ui->options->setCanReplace(false);
    int searchId = browser->searchId();
    if (searchId < 0) {
        return;
    }
    emit canReplace(searchId);
#endif
}

void SessionWidget::onFound(int searchId, SearchHits hits, SearchNameHits nameHits)
{
    SearchTab* tab = find(searchId);
    if (!tab) {
        qDebug() << "onFound find(searchId) == 0";
        return;
    }
    tab->append(hits, nameHits);

    ui->progressGroup->show();

    if (hits.complete() > -1) {
        ui->progress->progress(hits.complete(),hits.total(),hits.filtered(),hits.last());
    }

    if (!mCanceled.contains(searchId)) {
        emit searchMore(searchId);
    } else {
        ui->progressGroup->hide();
    }

    updateReplaceButton();
}

void SessionWidget::onPathChanged(QString path)
{
    if (!mTabWidget) {
        qDebug() << "cannot cast";
        return;
    }
    if (path.isEmpty()) {
        return;
    }

    //QLineEdit* edit = ui->options->pathEdit();


#if 0
    if (QDir(path).exists()) {
        //qDebug() << "getAllFiles" << path;
        emit getAllFiles(path);
    }
#endif
    mTabWidget->setTabText(mTabWidget->indexOf(this),QFileInfo(path).fileName());
}

#include "renamedialog.h"
#include "renameparams.h"

void SessionWidget::onReplaced(ReplacedParams params) {
    ui->progressGroup->show();
    ui->progress->replaced(params.countFiles(), params.countLines());

    SearchTab* tab = currentTab();
    tab->hits().clearCache();

    setMode(Mode::Replace);
    updateReplaceButton();

    if (params.renames().isEmpty())  {
        return;
    }

    RenameDialog dialog(params.renames());
    if (dialog.exec() == QDialog::Accepted) {

        auto checked = dialog.checked();
        emit rename(checked);
    }

}



void SessionWidget::onListing(QString path, QStringList files) {

    qDebug() << "SessionWidget::onListing" << path;

    if (ui->options->path() != path) {
        return;
    }

    bool update = !mAllFiles.contains(path) || mAllFiles[path] != files;

    mAllFiles[path] = files;

    if (!update) {
        return;
    }

    QCompleter* completer = ui->open->completer();
    if (completer) {
        completer->deleteLater();
    }

    QStandardItemModel* model = CompleterHelper::filesToModel(files, ui->open);
    completer = CompleterHelper::modelToCompleter(model, 0, ui->open);
    CompleterHelper::completerTreeViewPopup(completer, ui->open);

    ui->open->setCompleter(completer);

    if (!ui->open->text().isEmpty()) {
        if (!completer->popup()->isVisible()) {
            //completer->popup()->show();

            completer->setCompletionPrefix(ui->open->text());

            completer->complete();

        }
    }

    connect(completer, SIGNAL(activated(QModelIndex)), this, SLOT(onCompleterActivated(QModelIndex)));
}

void SessionWidget::onCompleterActivated(QModelIndex index) {

    const QAbstractItemModel* model = index.model();
    //qDebug() << "data" << model->data(index) << model->data(model->index(index.row(),1));

    QString path = model->data(model->index(index.row(),1)).toString();
    QUrl url = QUrl::fromLocalFile(path);
    mClickHandler->onAnchorClicked(url);
}

void SessionWidget::onGetListing() {
    QString path = ui->options->path();
    if (ui->options->cacheFileListChecked() && mAllFiles.contains(path)) {
        return;
    }
    emit getListing(GetListingParams(path, ui->options->cacheFileListChecked()));
    qDebug() << "emit getListing(path)";
}

void SessionWidget::onRenamed(int successful, int failed) {
    ui->progressGroup->show();
    ui->progress->renamed(successful, failed);
}

void SessionWidget::on_clear_clicked()
{
    ui->open->clear();
}
