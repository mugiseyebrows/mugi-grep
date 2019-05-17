#ifndef SESSIONWIDGET_H
#define SESSIONWIDGET_H

#include <QWidget>
#include <QUrl>
#include <QMetaType>
#include <QMap>

#include "regexp.h"
#include "regexppath.h"
#include "widget/regexpbaseinput.h"
#include "worker.h"
#include "searchparams.h"

namespace Ui {
class SessionWidget;
}

class File;
class Worker;
class SearchBrowser;
class QTabWidget;
class AnchorClickHandler;
class SearchOptionsWidget;

/*
Q_DECLARE_METATYPE(RegExp)
Q_DECLARE_METATYPE(RegExpPath)*/

class SessionWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SessionWidget(QWidget *parent = nullptr);
    ~SessionWidget();

    void cancelAll();

    QString path() const;

    void updateCompletions();

    void serialize(QJsonObject &json) const;
    void deserialize(const QJsonObject &v);
    SearchBrowser* find(int searchId);
    SearchBrowser *tab(int index);
    void countMatchedFiles();
    SearchBrowser *currentTab();
    int oldestTabIndex();
    SearchOptionsWidget *options() const;
protected:

    Worker* mWorker;
    QThread* mThread;

    static void save(const QString& path, const QString& text);

    bool mCancel;

    QTabWidget* mTabWidget;

    AnchorClickHandler* mClickHandler;

    SearchBrowser *createTab(const QString &name, SearchBrowser *browser = 0, bool append = true, bool select = true);

    bool mSetValues;
    void save(bool plain);
    void searchOrReplace(Worker::Action action);
signals:

    void search(SearchParams);
    void search(int action, int searchId, QString path, RegExpPath filter, bool notBinary, RegExp search, int linesBofore, int linesAfter, bool cacheFileList, QString);
    void searchMore(int searchId);
    void finishSearch(int searchId);
    //void collected();
    void collect();
    void replace(int);

public slots:
    void onCanceled();

protected slots:

    void on_saveText_clicked();
    void on_saveHtml_clicked();
    void on_results_currentChanged(int index);
    void onFound(int, QString, int, int, int, QString path);
    void onClone();
    void onSearch();
    void onTabTitle(QString title, bool isExecuted);
    void onPathChanged(QString path);

    void onPreview();
    void onReplace();
private:
    Ui::SessionWidget *ui;
};

#endif // SESSIONWIDGET_H
