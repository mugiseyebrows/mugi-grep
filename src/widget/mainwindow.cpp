#include "widget/mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QCloseEvent>
#include <QFileInfo>
#include <QSignalMapper>
#include <QDebug>

#include <QFileDialog>
#include <QMessageBox>
#include <QVariantMap>
#include <QMimeData>

#include "sessionwidget.h"
#include "settingsdialog.h"
#include "jsonhelper.h"
#include "settings.h"
#include "rxcollector.h"
#include <QJsonArray>
#include "anchorclickhandler.h"
#include "completermodelmanager.h"

MainWindow::MainWindow(Settings *settings, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), mMapper(new QSignalMapper(this)),
    mClickHandler(new AnchorClickHandler(settings, this)),
    mCompleterModelManager(new CompleterModelManager(this)),
    mSettings(settings)
{
    ui->setupUi(this);

    while (ui->tabs->count() > 0) {
        ui->tabs->removeTab(0);
    }

    setWindowTitle(QString("%1 %2").arg(qApp->applicationName()).arg(qApp->applicationVersion()));

    if (0) {

    } else {

        mViewOptions = deserealizeViewOptions(mSettings->viewOptions());

        RXCollector* collector = RXCollector::instance();

        collector->deserializePatterns(mSettings->patterns());
        collector->deserializePaths(mSettings->paths());

        QJsonArray sessions = mSettings->sessions();
        if (sessions.size() > 0) {
            deserealizeSessions(sessions);
        } else {
            addSession();
        }

        initViewOptionsMenu();
    }

    connect(ui->tabs,SIGNAL(tabCloseRequested(int)),this,SLOT(onTabClose(int)));

    //qDebug() << 1;

    //connect(mMapper,SIGNAL(mapped(QWidget*)),this,SLOT(onReadStarted(QWidget*)));
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

    if (0) {

    } else {
        mSettings->setSessions(serializeSessions());
        mSettings->setPatterns(RXCollector::instance()->serializePatterns());
        mSettings->setPaths(RXCollector::instance()->serializePaths());
        mSettings->setViewOptions(serializeViewOptions());
        mSettings->save();
    }

    e->accept();
}

void MainWindow::addSession(const QJsonValue &v) {
    SessionWidget* session = new SessionWidget(mSettings, ui->tabs);
    //session->setCacheFileList(ui->cacheFileList);

    //connect(session,SIGNAL(setEditor()),this,SLOT(onSetEditor()));
    //connect(this,SIGNAL(editorSet()),session,SLOT(onEditorSet()));

    // todo: tab name collisions
    QString title = "untitled";
    QString path = v.toString();

    if (!path.isEmpty()) {
        title = QFileInfo(path).baseName();
        RXCollector::instance()->collectPath(path);
    }
    ui->tabs->addTab(session, title);
    mMapper->setMapping(session,session);

    session->deserialize(v);

    session->loadCollected();

    session->setViewOptions(mViewOptions);

    connect(session, SIGNAL(viewOptionsChanged(ViewOptions)),this,SLOT(onViewOptionsChanged(ViewOptions)));

    /*connect(session,&SessionWidget::collect,[=](){
        mCompleterModelManager->onCollect(session->options(), ui->tabs);
    });*/
}

void MainWindow::onViewOptionsChanged(ViewOptions options) {
    mViewOptions = options;
    auto* tab = currentTab();
    if (!tab) {
        return;
    }

    mViewOptionCache->setChecked(options.cache());
    mViewOptionSearch->setChecked(options.search());
    mViewOptionFilter->setChecked(options.filter());
    mViewOptionDisplay->setChecked(options.display());
    mViewOptionNavigate->setChecked(options.navigate());
    mViewOptionAll->setChecked(options.all());

    tab->setViewOptions(options);
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

SessionWidget *MainWindow::currentTab() {
    return tab(ui->tabs->currentIndex());
}

QJsonArray MainWindow::serializeSessions() const
{
    QJsonArray result;
    for(int i=0;i<ui->tabs->count();i++) {
        QWidget* w = ui->tabs->widget(i);
        SessionWidget* sw = qobject_cast<SessionWidget*>(w);
        if (!sw)
            continue;
        result.append(sw->serialize());
    }
    return result;
}

QJsonObject MainWindow::serializeViewOptions() const {
    ViewOptions opt = mViewOptions;
    QJsonObject obj;
    obj["search"] = opt.search();
    obj["filter"] = opt.filter();
    obj["display"] = opt.display();
    obj["navigate"] = opt.navigate();
    obj["cache"] = opt.cache();
    return obj;
}

ViewOptions MainWindow::deserealizeViewOptions(const QJsonObject& obj) {
    ViewOptions opt;
    opt.setSearch(obj["search"].toBool());
    opt.setFilter(obj["filter"].toBool());
    opt.setDisplay(obj["display"].toBool());
    opt.setNavigate(obj["navigate"].toBool());
    opt.setCache(obj["cache"].toBool());
    return opt;
}


void MainWindow::deserealizeSessions(const QJsonArray& vl)
{
    while(ui->tabs->count()>0) {
        removeSession();
    }
    for(const QJsonValue& v : vl) {
        addSession(v);
    }
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

    QJsonArray sessions = serializeSessions();
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
    mClickHandler->onSetEditor(QString());
}

#if 0
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
#endif

void MainWindow::on_tabs_currentChanged(int index)
{
    SessionWidget* session = tab(index);
    if (!session) {
        qDebug() << "not SessionWidget at index" << index << ui->tabs->widget(index);
        return;
    }
    session->loadCollected();
    QTimer::singleShot(0,[=](){
        session->options()->fixLayout();
    });
    session->setViewOptions(mViewOptions);
}

void MainWindow::on_removeAllSessions_triggered()
{
    while (ui->tabs->count() > 0) {
        on_removeSession_triggered();
    }
    addSession();
}

void MainWindow::setCurrentTabMode(Mode mode) {
    SessionWidget* tab = currentTab();
    if (!tab) {
        return;
    }
    tab->setMode(mode);
}

#include <QAction>

void MainWindow::initViewOptionsMenu()
{
    ViewOptions options = mViewOptions;

    QMenu* menu = ui->menuView;

    mViewOptionCache = menu->addAction("Cache");
    mViewOptionCache->setCheckable(true);
    mViewOptionCache->setChecked(options.cache());

    mViewOptionSearch = menu->addAction("Find");
    mViewOptionSearch->setCheckable(true);
    mViewOptionSearch->setChecked(options.search());

    mViewOptionFilter = menu->addAction("File filter");
    mViewOptionFilter->setCheckable(true);
    mViewOptionFilter->setChecked(options.filter());

    mViewOptionDisplay = menu->addAction("Display");
    mViewOptionDisplay->setCheckable(true);
    mViewOptionDisplay->setChecked(options.display());

    mViewOptionNavigate = menu->addAction("Navigate");
    mViewOptionNavigate->setCheckable(true);
    mViewOptionNavigate->setChecked(options.navigate());

    mViewOptionAll = menu->addAction("All");
    mViewOptionAll->setCheckable(true);
    mViewOptionAll->setChecked(options.all());

    connect(mViewOptionCache,&QAction::triggered,[=](bool checked){
        mViewOptions.setCache(checked);
        onViewOptionsChanged(mViewOptions);
    });
    connect(mViewOptionSearch,&QAction::triggered,[=](bool checked){
        mViewOptions.setSearch(checked);
        onViewOptionsChanged(mViewOptions);
    });
    connect(mViewOptionFilter,&QAction::triggered,[=](bool checked){
        mViewOptions.setFilter(checked);
        onViewOptionsChanged(mViewOptions);
    });
    connect(mViewOptionDisplay,&QAction::triggered,[=](bool checked){
        mViewOptions.setDisplay(checked);
        onViewOptionsChanged(mViewOptions);
    });
    connect(mViewOptionNavigate,&QAction::triggered,[=](bool checked){
        mViewOptions.setNavigate(checked);
        onViewOptionsChanged(mViewOptions);
    });
    connect(mViewOptionAll,&QAction::triggered,[=](bool checked){
        mViewOptions.setAll(checked);
        onViewOptionsChanged(mViewOptions);
    });

}


void MainWindow::on_search_triggered()
{
    setCurrentTabMode(Mode::Search);
}

void MainWindow::on_replace_triggered()
{
    setCurrentTabMode(Mode::Preview);
}

void MainWindow::on_select_triggered()
{
    SessionWidget* tab = currentTab();
    if (!tab) {
        return;
    }
    tab->select();
}



void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    QUrl url(event->mimeData()->text());
    if (url.isValid() && url.scheme() == "file")
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event) {
    QString fileName = QUrl(event->mimeData()->text()).toLocalFile();
    if (!QDir(fileName).exists()) {
        return;
    }
    QJsonObject data;
    data["path"] = fileName;
    addSession(data);
    ui->tabs->setCurrentIndex(ui->tabs->count()-1);
}

void MainWindow::onSave(Format format) {
    SessionWidget* tab = currentTab();
    if (!tab) {
        return;
    }
    tab->save(format);
}

void MainWindow::on_saveAsText_triggered()
{
    onSave(Format::Text);
}

void MainWindow::on_saveAsHtml_triggered()
{
    onSave(Format::Html);
}


QString colorRepr(const QColor& color) {
    int r,g,b,a;
    color.getRgb(&r,&g,&b,&a);
    if (a == 255) {
        return QString("QColor(%1, %2, %3)").arg(r).arg(g).arg(b);
    }
    return QString("QColor(%1, %2, %3, %4)").arg(r).arg(g).arg(b).arg(a);
}

QString setPaletteColor(const QString& name, QPalette::ColorGroup group, QPalette::ColorRole role, const QColor& color) {

    static QStringList roles_ = {
        "QPalette::WindowText",
        "QPalette::Button",
        "QPalette::Light",
        "QPalette::Midlight",
        "QPalette::Dark",
        "QPalette::Mid",
        "QPalette::Text",
        "QPalette::BrightText",
        "QPalette::ButtonText",
        "QPalette::Base",
        "QPalette::Window",
        "QPalette::Shadow",
        "QPalette::Highlight",
        "QPalette::HighlightedText",
        "QPalette::Link",
        "QPalette::LinkVisited",
        "QPalette::AlternateBase",
        "QPalette::NoRole",
        "QPalette::ToolTipBase",
        "QPalette::ToolTipText",
        "QPalette::PlaceholderText"
    };

    static QStringList groups_ = {
        "QPalette::Active",
        "QPalette::Disabled",
        "QPalette::Inactive"
    };

    if (group == QPalette::All) {
        return QString("%1.setColor(%2,%3);").arg(name).arg(roles_[role]).arg(colorRepr(color));
    }
    return QString("%1.setColor(%2,%3,%4);").arg(name).arg(groups_[group]).arg(roles_[role]).arg(colorRepr(color));
}

void dumpPalette(const QString& fileName) {

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "cannot open" << fileName << "for writing";
        return;
    }
    QTextStream stream(&file);

    QList<QPalette::ColorGroup> groups = {QPalette::Active,
                                          QPalette::Disabled,
                                          QPalette::Inactive};
    QStringList groups_ = {
        "QPalette::Active",
        "QPalette::Disabled",
        "QPalette::Inactive"
    };

    QList<QPalette::ColorRole> roles = {QPalette::WindowText,
                                        QPalette::Button,
                                        QPalette::Light,
                                        QPalette::Midlight,
                                        QPalette::Dark,
                                        QPalette::Mid,
                                        QPalette::Text,
                                        QPalette::BrightText,
                                        QPalette::ButtonText,
                                        QPalette::Base,
                                        QPalette::Window,
                                        QPalette::Shadow,
                                        QPalette::Highlight,
                                        QPalette::HighlightedText,
                                        QPalette::Link,
                                        QPalette::LinkVisited,
                                        QPalette::AlternateBase,
                                        QPalette::NoRole,
                                        QPalette::ToolTipBase,
                                        QPalette::ToolTipText,
                                        QPalette::PlaceholderText
                                       };

    QStringList roles_ = {
        "QPalette::WindowText",
        "QPalette::Button",
        "QPalette::Light",
        "QPalette::Midlight",
        "QPalette::Dark",
        "QPalette::Mid",
        "QPalette::Text",
        "QPalette::BrightText",
        "QPalette::ButtonText",
        "QPalette::Base",
        "QPalette::Window",
        "QPalette::Shadow",
        "QPalette::Highlight",
        "QPalette::HighlightedText",
        "QPalette::Link",
        "QPalette::LinkVisited",
        "QPalette::AlternateBase",
        "QPalette::NoRole",
        "QPalette::ToolTipBase",
        "QPalette::ToolTipText",
        "QPalette::PlaceholderText"
    };

    QPalette palette = qApp->palette();

    for(QPalette::ColorRole role: roles) {
        QColor c1 = palette.color(QPalette::Active, role);
        QColor c2 = palette.color(QPalette::Disabled, role);
        QColor c3 = palette.color(QPalette::Inactive, role);
        QString name = "palette";
        if (c1 == c2 && c2 == c3) {
            stream << setPaletteColor(name, QPalette::All, role, c1) << "\n";
        } else {
            stream << setPaletteColor(name, QPalette::Active, role, c1) << "\n";
            stream << setPaletteColor(name, QPalette::Disabled, role, c2) << "\n";
            stream << setPaletteColor(name, QPalette::Inactive, role, c3) << "\n";
        }
    }

    stream.flush();

    qDebug() << fileName << "writen";
}

#include "stylehelper.h"

void MainWindow::on_lightStyle_triggered()
{
    //dumpPalette("D:\\w\\light-palette.txt");
    StyleHelper::setLightStyle();
    mSettings->setStyle("light");
}

void MainWindow::on_darkStyle_triggered()
{
    StyleHelper::setDarkStyle();
    //dumpPalette("D:\\w\\dark-palette.txt");
    mSettings->setStyle("dark");
}

void MainWindow::onTabClose(int index) {
    if (ui->tabs->count() <= 1) {
        return;
    }
    SessionWidget* session = tab(index);
    if (!session) {
        return;
    }
    session->onCanceled();
    ui->tabs->removeTab(index);
    session->deleteLater();
}
