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
#include "utils/json.h"

#define RESULT_TAB_LIMIT 10

QStringList spaceSplit(const QString& s_) {
    QStringList res;
    QString s = s_;
    int p = s.indexOf(" ");
    while(p > -1) {
        QString m = s.mid(0,p);
        //qDebug() << "m" << m;
        if (m.startsWith('"') == m.endsWith('"')) {
            if (m.startsWith('"')) {
                res << m.mid(1,m.size()-2);
            } else {
                res << m;
            }
            s = s.mid(p+1);
            p = -1;
        }
        p = s.indexOf(" ",p+1);
    }
    res << s;
    return res;
}

SessionWidget::SessionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SessionWidget)
{
    ui->setupUi(this);

    mParent = qobject_cast<QTabWidget*>(parent);

    mWorker = new Worker();
    mThread = new QThread();

    mWorker->moveToThread(mThread);

    connect(this,SIGNAL(search(int,QString,RegExpPath,bool,RegExp,int,int)),
            mWorker,SLOT(onSearch(int,QString,RegExpPath,bool,RegExp,int,int)));
    connect(mWorker,SIGNAL(found(int,QString,int,int,int,QString)),
            this,SLOT(onFound(int,QString,int,int,int,QString)));
    connect(this,SIGNAL(searchMore(int)),mWorker,SLOT(onSearchMore(int)));

    connect(this,SIGNAL(finishSearch(int)),mWorker,SLOT(onFinishSearch(int)));

    connect(mWorker,SIGNAL(sizeCalculated(int,int)),this,SLOT(onSizeCalculated(int,int)));

    mThread->start();

    ui->progressBar->hide();
    ui->cancel->hide();

    while(ui->results->count() > 0) {
        ui->results->removeTab(0);
    }

    ui->status2->hide();

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


void SessionWidget::cancelAll()
{

}


QString SessionWidget::readPath() const
{
    return ui->path->text();
}

void SessionWidget::serialize(QJsonObject& json) const
{
    json["path"] = ui->path->text();
}

void SessionWidget::deserialize(const QJsonObject &v)
{
    ui->path->setText(v.value("path").toString());
}

void SessionWidget::startRead()
{
    //on_read_clicked();
}

void SessionWidget::updateCollector()
{
    RXCollector::instance()->load(ui->searchFilter);
    RXCollector::instance()->load(ui->searchExp);
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

void SessionWidget::on_linesAfter_returnPressed() {
    on_search_clicked();
}

void SessionWidget::on_linesBefore_returnPressed() {
    on_search_clicked();
}



void SessionWidget::on_search_clicked() {

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

    // add tab

    mSearchId = SearchId::instance()->next();
    SearchBrowser* browser = new SearchBrowser(ui->searchExp->value(),ui->searchFilter->value(),ui->linesBefore->value(),ui->linesAfter->value());
    mResults[mSearchId] = browser;

    connect(browser,SIGNAL(anchorClicked(QUrl)),this,SLOT(onAnchorClicked(QUrl)));
    QString title = ui->searchExp->value().include();

    //qDebug() << "title" << title << ui->searchExp->value().exps();

    int maxTitleChars = 20;
    if (title.size() > maxTitleChars) {
        title = title.mid(0,maxTitleChars-1) + "…";
    }

    ui->results->insertTab(0,browser,title);
    ui->results->setCurrentIndex(0);

    //qDebug() << "mResults[mSearchId] = browser" << mSearchId << browser;

    emit search(mSearchId,path,filter,notBinary,search_,linesBefore,linesAfter);

    ui->status->setText(QString("Building path list"));

    RXCollector::instance()->collect(filter);
    RXCollector::instance()->collect(search_);
    updateCollector();
}

void SessionWidget::on_selectPath_clicked() {

    QString path = QFileDialog::getExistingDirectory(this, QString(), ui->path->text());
    if (path.isEmpty())
        return;

    ui->path->setText(QDir::toNativeSeparators(path));
    //on_read_clicked();
}

void SessionWidget::on_searchExp_returnPressed() {
    on_search_clicked();
}

void SessionWidget::on_searchFilter_returnPressed() {
    on_search_clicked();
}

namespace {
QString unquote(const QString& text) {
    if (text.startsWith('"') && text.endsWith('"')) {
        return text.mid(1,text.size()-2);
    }
    return text;
}

}

void SessionWidget::onAnchorClicked(QUrl url) {

    QUrlQuery q(url);

    QString line = q.queryItemValue("line");

    QString path = url.path();
    if (path.startsWith("/"))
        path = path.mid(1);

    QString editor = Settings::instance()->editor(path);
    if (editor.isEmpty()) {
        QString question = "Editor not set for this file type, set editor?";
        if (QMessageBox::question(this,"Editor not set",question, QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
            mQueued = url;
            emit setEditor();
        }
    } else {

        QStringList editor_ = spaceSplit(editor);
        QString opt;

        QStringList args;
        //args << path;

        bool pathSpecified = false;

        for(int i=1;i<editor_.size();i++) {
            QString opt = editor_[i];
            if (opt.indexOf("%file%") > -1) {
                opt = opt.replace("%file%",path);
                pathSpecified = true;
            }
            if (opt.indexOf("%line%") > -1) {
                opt = opt.replace("%line%",line);
            }
            args << opt;
        }
        if (!pathSpecified)
            args << path;

        QString app = unquote(editor_[0]);

        bool ok = QProcess::startDetached(app, args);
        if (!ok) {
            QMessageBox::critical(this,QString("Error"),QString("Failed to start %1").arg(editor_[0]));
        }
    }
}

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

void SessionWidget::onFound(int id, QString res, int i, int t, int s, QString path)
{
    SearchBrowser* browser;

    browser = mResults.value(id);

    //qDebug() << "onFound" << id;

    /*if (id != mSearchId) {
        qDebug() << "MainWindow::onFound id != mSearchId";
        return;
    }*/

    if (!browser) {
        qDebug() << "onFound error no browser";
        return;
    }

    if (!res.isEmpty())
        browser->append(res);

    if (i >= 0) {
        ui->status->setText(QString("Processed %1 files out of %2 (%3 filtered out) last file: %4").arg(i).arg(t).arg(s).arg(path));
        ui->progressBar->setMaximum(t);
        ui->progressBar->setValue(i);
        ui->progressBar->setVisible( i != t );
        //qDebug() << i << t << s;
    }
    ui->cancel->setVisible( i != t );

    //emit searchMore(id);

    if (i < t && !mCancel) {
        //qDebug() << "i < t" << i << t;
        emit searchMore(id);
    } else {
        //emit calcSize();
        emit finishSearch(id);
    }

    if (mCancel) {
        ui->progressBar->setVisible(false);
        ui->cancel->setVisible(false);
        ui->status->setText(QString("Search aborted"));
    }
}

void SessionWidget::onEditorSet()
{
    if (mQueued.isEmpty())
        return;
    onAnchorClicked(mQueued);
    mQueued = QUrl();
}

void SessionWidget::on_results_currentChanged(int index) {

    QWidget* w = ui->results->widget(index);
    SearchBrowser* b = qobject_cast<SearchBrowser*>(w);
    if (!b) {
        qDebug() << "not a browser at index" << index;
        return;
    }

    if (!mResults.values().contains(b)) {
        qDebug() << "not in mResults" << b;
        return;
    }

    ui->searchFilter->setValue(b->filter());
    ui->searchExp->setValue(b->exp());
    ui->linesBefore->setValue(b->linesBefore());
    ui->linesAfter->setValue(b->linesAfter());
}

void SessionWidget::onSizeCalculated(int size,int keySize) {
    ui->status2->setText(QString("%1 %2").arg(size).arg(keySize));
}

void SessionWidget::on_path_textChanged(const QString &path)
{
    if (!mParent) {
        qDebug() << "cannot cast";
        return;
    }
    mParent->setTabText(mParent->indexOf(this),QFileInfo(path).fileName());
}
