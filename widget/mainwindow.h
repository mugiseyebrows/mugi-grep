#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include <QMetaType>
#include <QJsonObject>
#include "searchoptionswidget.h"

class SessionWidget;
class QSignalMapper;
class AnchorClickHandler;
class CompleterModelManager;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *);

    void addSession(const QJsonObject &v = QJsonObject());
    void removeSession();

    SessionWidget* tab(int index);
    SessionWidget *currentTab();

    void deserealizeSessions(const QJsonArray &vl);
    void serializeExps(QJsonObject &json) const;
    void deserealizeExps(const QJsonObject &exps);
    void serializeSessions(QJsonArray &json) const;
    void setCurrentTabMode(SearchOptionsWidget::Mode mode);


protected:
    Ui::MainWindow *ui;
    QSignalMapper* mMapper;
    AnchorClickHandler* mClickHandler;
    CompleterModelManager* mCompleterModelManager;


signals:
    void editorSet();

protected slots:
    //QList<SessionWidget*> mSessions;

    void on_addSession_triggered();
    void on_removeSession_triggered();
    void on_saveSessions_triggered();
    void on_loadSessions_triggered();

    void on_setEditors_triggered();

    void onReadStarted(QWidget *);

    void on_tabs_currentChanged(int index);

    void on_removeAllSessions_triggered();

    void on_search_triggered();
    void on_replace_triggered();
    void on_select_triggered();
};

#endif // MAINWINDOW_H
