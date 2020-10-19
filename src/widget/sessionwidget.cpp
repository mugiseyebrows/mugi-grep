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
#include "rxcollector.h"
#include "searchid.h"
#include "settings.h"
#include <QMessageBox>
#include "anchorclickhandler.h"
#include "widget/selectfilesdialog.h"
#include "widget/mainwindow.h"

#include <QCompleter>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QTreeView>
#include "searchtab.h"
#include "fileio.h"
#include "callonce.h"
#include "countfilesmanager.h"

#define RESULT_TAB_LIMIT 10

SessionWidget::SessionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SessionWidget),
    mClickHandler(new AnchorClickHandler()),
    mListenOptions(false),
    /*mReplacementChanged(new CallOnce("mReplacementChanged", 500, this)),*/
    mGetListing(new CallOnce("mGetAllFiles", 500, this)),
    mCountFilesManager(new CountFilesManager(this))
{
    ui->setupUi(this);

    mTabWidget = qobject_cast<QTabWidget*>(parent);

    mWorker = new Worker();
    mThread = new QThread();

    mWorker->moveToThread(mThread);

    /*connect(this,SIGNAL(search(int,int,QString,RegExpPath,bool,RegExp,int,int,bool,QString)),
            mWorker,SLOT(onSearch(int,int,QString,RegExpPath,bool,RegExp,int,int,bool,QString)));*/

#if 0
    connect(this,SIGNAL(search(SearchParams)),mWorker,SLOT(onSearch(SearchParams)));

    connect(this,SIGNAL(replace(int)),mWorker,SLOT(onReplace(int)));
    connect(mWorker,SIGNAL(found(int,QString,int,int,int,QString)),
            this,SLOT(onFound(int,QString,int,int,int,QString)));
    connect(this,SIGNAL(searchMore(int)),mWorker,SLOT(onSearchMore(int)));
    connect(this,SIGNAL(finishSearch(int)),mWorker,SLOT(onFinishSearch(int)));
    connect(ui->options,SIGNAL(clone()),this,SLOT(onClone()));
    connect(ui->options,SIGNAL(search()),this,SLOT(onSearch()));
    connect(ui->options,SIGNAL(preview()),this,SLOT(onPreview()));
    connect(ui->options,SIGNAL(replace()),this,SLOT(onReplace()));
    connect(ui->options,SIGNAL(tabTitle(QString,bool)),this,SLOT(onTabTitle(QString,bool)));
    connect(ui->options,SIGNAL(pathChanged(QString)),this,SLOT(onPathChanged(QString)));
    connect(ui->progress,SIGNAL(canceled()),this,SLOT(onCanceled()));

    connect(this,SIGNAL(canReplace(int)),mWorker,SLOT(onCanReplace(int)));
    connect(mWorker,SIGNAL(canReplace(int,bool)),this,SLOT(onCanReplace(int,bool)));
    connect(mWorker,SIGNAL(replaced(int,int,int,QStringList)),this,SLOT(onReplaced(int,int,int,QStringList)));

    connect(this,SIGNAL(getAllFiles(QString)),mWorker,SLOT(onGetAllFiles(QString)));
    connect(mWorker,SIGNAL(allFiles(QString,QStringList)),this,SLOT(onAllFiles(QString,QStringList)));

   #endif

    connect(this,SIGNAL(search(SearchParams)),mWorker,SLOT(onSearch(SearchParams)));
    connect(this,SIGNAL(searchMore(int)),mWorker,SLOT(onSearchMore(int)));
    connect(mWorker,SIGNAL(found(int,SearchHits)),this,SLOT(onFound(int,SearchHits)));

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
    connect(mWorker,SIGNAL(replaced(int,int)),this,SLOT(onReplaced(int,int)));

    //connect(mReplacementChanged,SIGNAL(call()),this,SLOT(onPreview()));
    //connect(mCountFiles,SIGNAL(call()),this,SLOT(onCountFiles()));
    connect(mGetListing,SIGNAL(call()),this,SLOT(onGetListing()));
    connect(this,SIGNAL(getListing(GetListingParams)),mWorker,SLOT(onGetListing(GetListingParams)));
    connect(mWorker,SIGNAL(listing(QString,QStringList)),this,SLOT(onListing(QString,QStringList)));

    connect(mCountFilesManager,SIGNAL(filesCounted()),this,SLOT(onFilesCounted()));
    connect(mCountFilesManager,SIGNAL(countFiles(CountFilesParams)),mWorker,SLOT(onCountFiles(CountFilesParams)));
    connect(mWorker,SIGNAL(filesCounted(CountFilesParams)),mCountFilesManager,SLOT(onFilesCounted(CountFilesParams)));

    connect(ui->options->cacheFileList(), SIGNAL(clicked(bool)), this, SLOT(onCacheFileListClicked(bool)));

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

    if (ui->options->cacheFileListIsChecked()) {
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
    if (!ui->options->cacheFileListIsChecked()) {
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
    //searchOrReplace(Worker::Search);

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
    tab->params().setCacheFileList(ui->options->cacheFileListIsChecked());
    tab->hits().clear();
    tab->trigRerender();

    updateTabText(ui->results->currentIndex());

    emit search(tab->params());
    ui->progressGroup->show();
    ui->progress->started();
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
    ReplaceParams params = tab->replaceParams();
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

void SessionWidget::serialize(QJsonObject& json) const
{
    json["path"] = ui->options->path();
}

void SessionWidget::deserialize(const QJsonObject &v)
{
    ui->options->setPath(v.value("path").toString());
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

#if 0
void SessionWidget::onFound(int searchId, QString res, int i, int t, int s, QString path)
{

    //qDebug() << searchId << res.size() << "chars" << i << t << s << path;

    SearchBrowser* browser = find(searchId);

    if (!browser) {
        //qDebug() << "onFound error no browser";
        return;
    }

    if (!res.isEmpty()) {
        browser->append(res);
    }

    ui->progressGroup->show();

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
#endif

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

    if (ui->options->cacheFileListIsChecked()) {
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

void SessionWidget::onFound(int searchId, SearchHits hits)
{
    SearchTab* tab = find(searchId);
    if (!tab) {
        qDebug() << "onFound find(searchId) == 0";
        return;
    }
    tab->append(hits);

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
#if 0
    if (QDir(path).exists()) {
        //qDebug() << "getAllFiles" << path;
        emit getAllFiles(path);
    }
#endif
    mTabWidget->setTabText(mTabWidget->indexOf(this),QFileInfo(path).fileName());
}
#if 0
void SessionWidget::onReplaced(int searchId,int files,int lines,QStringList notChanged) {

    SearchBrowser* browser = currentTab();
    if (browser->searchId() != searchId) {
        return;
    }
    onCanReplace(searchId,false);
    ui->progress->replaced(files, lines);
    if (notChanged.isEmpty()) {
        return;
    }
    QMessageBox::critical(this,"Error",QString("Failed to replace text in files:\n%1").arg(notChanged.join("\n")));

}
#endif

void SessionWidget::onReplaced(int files,int lines) {
    ui->progressGroup->show();
    ui->progress->replaced(files, lines);

    SearchTab* tab = currentTab();
    tab->hits().clearCache();

    setMode(Mode::Replace);
    updateReplaceButton();
}

QString nameFromPath(const QString& path) {
    int p = qMax(path.lastIndexOf('/'), path.lastIndexOf('\\'));
    if (p < 0) {
        return path;
    }
    return path.mid(p + 1);
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

    completer = new QCompleter(ui->open);

    QStandardItemModel* model = new QStandardItemModel(files.size(), 2, completer);

    for(int r=0;r<model->rowCount();r++) {
        model->setData(model->index(r,0), nameFromPath(files[r]));
        model->setData(model->index(r,1), files[r]);
    }

    completer->setModel(model);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCaseSensitivity(Qt::CaseInsensitive);

    QTreeView* view = new QTreeView(ui->open);
    completer->setPopup(view);
    view->header()->hide();
    view->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    view->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    view->header()->setStretchLastSection(false);
    view->setRootIsDecorated(false);

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
    if (ui->options->cacheFileListIsChecked() && mAllFiles.contains(path)) {
        return;
    }
    emit getListing(GetListingParams(path, ui->options->cacheFileListIsChecked()));
    qDebug() << "emit getListing(path)";
}
