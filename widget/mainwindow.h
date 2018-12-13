#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include <QMetaType>
#include <QJsonObject>

class SessionWidget;
class QSignalMapper;

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

    void deserealizeSessions(const QJsonArray &vl);
    void serializeExps(QJsonObject &json) const;
    void deserealizeExps(const QJsonObject &exps);
    void serializeSessions(QJsonArray &json) const;
protected:
    Ui::MainWindow *ui;
    QSignalMapper* mMapper;

signals:
    void editorSet();

protected slots:
    //QList<SessionWidget*> mSessions;

    void on_addSession_triggered();
    void on_removeSession_triggered();
    void on_saveSessions_triggered();
    void on_loadSessions_triggered();


    void on_setEditors_triggered();

    void onSetEditor();

    void onReadStarted(QWidget *);


private slots:
    void on_tabs_currentChanged(int index);

    void on_removeAllSessions_triggered();

};

#endif // MAINWINDOW_H
