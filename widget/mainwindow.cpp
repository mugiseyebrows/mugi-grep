#include "widget/mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QFileInfo>
#include <QSignalMapper>
#include <QDebug>

#include <QFileDialog>
#include <QMessageBox>
#include <QVariantMap>

#include "widget/sessionwidget.h"

#include "widget/settingsdialog.h"
#include "utils/jsonhelper.h"
#include "settings.h"
#include "rxcollector.h"
#include <QJsonArray>
#include "anchorclickhandler.h"
#include "completermodelmanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), mMapper(new QSignalMapper(this)),
    mClickHandler(new AnchorClickHandler(this)), mCompleterModelManager(new CompleterModelManager(this))
{
    ui->setupUi(this);

    while (ui->tabs->count() > 0) {
        ui->tabs->removeTab(0);
    }

    setWindowTitle(QString("%1 %2").arg(qApp->applicationName()).arg(qApp->applicationVersion()));

    QJsonArray sessions = Settings::instance()->sessions();

    if (sessions.size() > 0) {
        deserealizeSessions(sessions);
    } else {
        addSession();
    }

    QJsonObject exps = Settings::instance()->exps();
    deserealizeExps(exps);

    connect(mMapper,SIGNAL(mapped(QWidget*)),this,SLOT(onReadStarted(QWidget*)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent * e)
{

    for(int i=0;i<ui->tabs->count();i++) {
        SessionWidget* session = tab(i);
        if (session) {
            session->onCanceled();
        }
    }

    QJsonArray sessions;
    serializeSessions(sessions);
    QJsonObject exps;
    serializeExps(exps);

    Settings::instance()->setSessions(sessions);
    Settings::instance()->setExps(exps);
    Settings::instance()->save();

    e->accept();
}

void MainWindow::addSession(const QJsonObject &v) {
    SessionWidget* session = new SessionWidget(ui->tabs);

    //connect(session,SIGNAL(setEditor()),this,SLOT(onSetEditor()));
    //connect(this,SIGNAL(editorSet()),session,SLOT(onEditorSet()));

    // todo: tab name collisions
    QString title = "untitled";
    QString path = v.value("path").toString();
    if (!path.isEmpty()) {
        title = QFileInfo(path).baseName();
    }
    ui->tabs->addTab(session,title);
    mMapper->setMapping(session,session);
    if (!v.isEmpty()) {
        session->deserialize(v);
    }
    session->updateCompletions();
    connect(session,&SessionWidget::collect,[=](){
        mCompleterModelManager->onCollect(session->options(), ui->tabs);
    });
}

void MainWindow::removeSession() {

    if (ui->tabs->count() == 0) {
        return;
    }

    int currentIndex = ui->tabs->currentIndex();
    SessionWidget* session = tab(currentIndex);
    if (session) {
        session->onCanceled();
    }
    ui->tabs->removeTab(currentIndex);
    session->deleteLater();
}

SessionWidget *MainWindow::tab(int index)
{
    return qobject_cast<SessionWidget*>(ui->tabs->widget(index));
}

void MainWindow::serializeSessions(QJsonArray& json) const
{
    for(int i=0;i<ui->tabs->count();i++) {
        QWidget* w = ui->tabs->widget(i);
        SessionWidget* sw = qobject_cast<SessionWidget*>(w);
        if (!sw)
            continue;
        QJsonObject session;
        sw->serialize(session);
        json << session;
    }
}

void MainWindow::serializeExps(QJsonObject& json) const {

    return RXCollector::instance()->serialize(json);
}

void MainWindow::deserealizeSessions(const QJsonArray& vl)
{
    while(ui->tabs->count()>0) {
        removeSession();
    }
    for(const QJsonValue& v : vl) {
        addSession(v.toObject());
    }
}

void MainWindow::deserealizeExps(const QJsonObject& exps) {
    RXCollector::instance()->deserialize(exps);
}

void MainWindow::on_addSession_triggered()
{
    addSession();
    ui->tabs->setCurrentIndex(ui->tabs->count()-1);
}

void MainWindow::on_removeSession_triggered()
{
    if (ui->tabs->count() == 0) {
        return;
    }
    int index = ui->tabs->currentIndex();
    SessionWidget* session = tab(index);
    if (!session) {
        return;
    }
    session->onCanceled();
    ui->tabs->removeTab(index);
    session->deleteLater();
}

void MainWindow::on_saveSessions_triggered() {

    QString path = QFileDialog::getSaveFileName(this,QString(),QString(),"json files (*.json)");

    if (path.isEmpty())
        return;

    QJsonArray sessions;
    serializeSessions(sessions);
    if (!saveJson(path, sessions)) {
        QMessageBox::critical(this,"Error","Cannot write file " + path);
    }
}

void MainWindow::on_loadSessions_triggered() {

    QString path = QFileDialog::getOpenFileName(this,QString(),QString(),"json files (*.json)");
    if (path.isEmpty()) {
        return;
    }

    bool ok;
    QJsonDocument doc = loadJson(path,&ok);

    if (!ok) {
        QMessageBox::critical(this,"Error","Cannot read file " + path);
        return;
    }

    if (!doc.isArray()) {
        QMessageBox::critical(this,"Error","Cannot parse file " + path);
        return;
    }

    while(ui->tabs->count()>0) {
        removeSession();
    }

    QJsonArray sessions = doc.array();

    for(const QJsonValue& session : sessions) {
        addSession(session.toObject());
    }
}

void MainWindow::on_setEditors_triggered()
{
    mClickHandler->onSetEditor();
}

void MainWindow::onReadStarted(QWidget* w) {

    //QFileInfo(path).baseName();
    SessionWidget* sw = qobject_cast<SessionWidget*>(w);
    if (!sw) {
        qDebug() << "onReadStarted wat";
        return;
    }

    QString name = QFileInfo(sw->path()).baseName();
    int index = ui->tabs->indexOf(w);
    if (index < 0) {
        qDebug() << "onReadStarted index < 0";
        return;
    }
    ui->tabs->setTabText(index,name);

}

void MainWindow::on_tabs_currentChanged(int index)
{
    /*SessionWidget* widget = tab(index);
    if (!widget) {
        qDebug() << "not SessionWidget at index" << index << ui->tabs->widget(index);
        return;
    }
    widget->updateCompletions();*/
}

void MainWindow::on_removeAllSessions_triggered()
{
    while (ui->tabs->count() > 0) {
        on_removeSession_triggered();
    }
}
