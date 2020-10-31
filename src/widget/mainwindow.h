#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include <QMetaType>
#include <QJsonObject>
#include "searchoptionswidget.h"
#include "mode.h"
#include "format.h"

class SessionWidget;
class QSignalMapper;
class AnchorClickHandler;
class CompleterModelManager;
class Settings;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(Settings* settings, QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *);

    void addSession(const QJsonValue &v = QJsonValue());
    void removeSession();

    SessionWidget* tab(int index);
    SessionWidget *currentTab();

    void deserealizeSessions(const QJsonArray &vl);
    void serializePatterns(QJsonObject &json) const;
    void deserealizePatterns(const QJsonObject &exps);
    QJsonArray serializeSessions() const;
    void setCurrentTabMode(Mode mode);

protected:

    Ui::MainWindow *ui;
    QSignalMapper* mMapper;
    AnchorClickHandler* mClickHandler;
    CompleterModelManager* mCompleterModelManager;
    Settings* mSettings;

signals:
    void editorSet();

protected slots:
    //QList<SessionWidget*> mSessions;

    void on_addSession_triggered();
    void on_removeSession_triggered();
    void on_saveSessions_triggered();
    void on_loadSessions_triggered();

    void on_setEditors_triggered();

    //void onReadStarted(QWidget *);

    void on_tabs_currentChanged(int index);

    void on_removeAllSessions_triggered();

    void on_search_triggered();
    void on_replace_triggered();
    void on_select_triggered();

    // QWidget interface
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    void onSave(Format format);
private slots:
    void on_saveAsText_triggered();
    void on_saveAsHtml_triggered();
    void on_lightStyle_triggered();
    void on_darkStyle_triggered();
};

#endif // MAINWINDOW_H
