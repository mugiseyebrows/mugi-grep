#include "anchorclickhandler.h"

#include <QProcess>
#include "settings.h"
#include <QMessageBox>
#include "widget/settingsdialog.h"
#include <QTextBrowser>
#include <QApplication>
#include <QUrlQuery>

AnchorClickHandler::AnchorClickHandler(QObject *parent) : QObject(parent)
{

}

void AnchorClickHandler::setBrowser(QTextBrowser *browser)
{
   connect(browser,SIGNAL(anchorClicked(QUrl)),this,SLOT(onAnchorClicked(QUrl)));
   connect(this,SIGNAL(setEditor()),this,SLOT(onSetEditor()));

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
    if (mQueued.isEmpty())
        return;
    onAnchorClicked(mQueued);
    mQueued = QUrl();
}


namespace  {

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

}

void AnchorClickHandler::onAnchorClicked(QUrl url) {


    QWidget* widget = qApp->activeWindow();

    QUrlQuery q(url);

    QString line = q.queryItemValue("line");

    if (line.isEmpty()) {
        line = "1";
    }

    QString path = url.path();
    if (path.startsWith("/")) {
        path = path.mid(1);
    }

    QString editor = Settings::instance()->editor(path);
    if (editor.isEmpty()) {
        QString question = "Editor not set for this file type, set editor?";
        if (QMessageBox::question(widget,"Editor not set",question, QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
            mQueued = url;
            emit setEditor();
        }
    } else {

        QStringList editor_ = spaceSplit(editor);

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
            QMessageBox::critical(widget,QString("Error"),QString("Failed to start %1").arg(editor_[0]));
        }
    }
}
