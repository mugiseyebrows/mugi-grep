#include "anchorclickhandler.h"

#include <QProcess>
#include "settings.h"
#include <QMessageBox>
#include "widget/settingsdialog.h"
#include <QTextBrowser>
#include <QApplication>
#include <QUrlQuery>
#include <QDebug>
#include "utils/fileutils.h"
#include <QAction>
#include <QMenu>
#include <QScrollBar>

namespace  {

QString urlPath(const QUrl& url) {
    QString path = url.path();
    if (path.startsWith("/")) {
        path = path.mid(1);
    }
    return path;
}

QString urlLine(const QUrl& url, const QString& defaultValue) {
    QUrlQuery q(url);
    QString line = q.queryItemValue("line");
    if (line.isEmpty()) {
        line = defaultValue;
    }
    return line;
}

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

QString unquote(const QString& text) {
    if (text.startsWith('"') && text.endsWith('"')) {
        return text.mid(1,text.size()-2);
    }
    return text;
}

QPair<QString,QStringList> toAppArgs(const QString& editor, const QString& path, const QString& line) {
    QStringList editor_ = spaceSplit(editor);
    QStringList args;
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
    if (!pathSpecified) {
        args << path;
    }
    QString app = unquote(editor_[0]);
    return QPair<QString,QStringList>(app,args);
}

} // namespace

AnchorClickHandler::AnchorClickHandler(QObject *parent) : QObject(parent)
{

}

void AnchorClickHandler::connectBrowser(QTextBrowser *browser)
{
   connect(browser,SIGNAL(anchorClicked(QUrl)),this,SLOT(onAnchorClicked(QUrl)));
   browser->setContextMenuPolicy(Qt::CustomContextMenu);
   connect(browser,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(onCustomContextMenuRequested(QPoint)));
}

void AnchorClickHandler::onSetEditor()
{
    QWidget* widget = qApp->activeWindow();

    SettingsDialog dialog(widget);
    if (dialog.exec() == QDialog::Accepted) {
        dialog.apply();
        QString error = Settings::instance()->error();
        if (!error.isEmpty()) {
            QMessageBox::critical(widget,"error",error);
        }
    }
}

void AnchorClickHandler::onAnchorClicked(QUrl url) {

    //qDebug() << "onAnchorClicked" << url;

    QString path = urlPath(url);
    QString line = urlLine(url,"1");

    QString editor = Settings::instance()->editor(path);
    if (editor.isEmpty()) {
        onSetEditor();
        editor = Settings::instance()->editor(path);
        if (!editor.isEmpty()) {
            onAnchorClicked(url);
            return;
        }
    } else {
        QPair<QString,QStringList> appArgs = toAppArgs(editor,path,line);
        bool ok = QProcess::startDetached(appArgs.first, appArgs.second);
        if (!ok) {
            QWidget* widget = qApp->activeWindow();
            QMessageBox::critical(widget,QString("Error"),QString("Failed to start %1").arg(appArgs.first));
        }
    }
}



void AnchorClickHandler::onCustomContextMenuRequested(QPoint point) {

    QTextBrowser* browser = qobject_cast<QTextBrowser*>(sender());
    if (!browser) {
        qDebug() << "AnchorClickHandler::onCustomContextMenuRequested error: sender is not a QTextBrowser*";
        return;
    }

    QPoint coordinateOffset(browser->horizontalScrollBar()->value(),
           browser->verticalScrollBar()->value());
    QMatrix matrix;
    matrix.translate(coordinateOffset.x(), coordinateOffset.y());
    QMenu* menu = browser->createStandardContextMenu(matrix.map(point));

    QString anchor = browser->anchorAt(point);

    QAction* show = new QAction("Show In File &Browser");
    show->setEnabled(!anchor.isEmpty());
    menu->insertAction(menu->actions().value(1),show);

    QAction* result = menu->exec(browser->mapToGlobal(point));

    if (result == show) {
        QString path = urlPath(QUrl(anchor));
        FileUtils::showInGraphicalShell(qApp->activeWindow(),path);
    }

    delete show;
}

