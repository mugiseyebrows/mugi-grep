#ifndef SESSIONWIDGET_H
#define SESSIONWIDGET_H

#include "mode.h"
#include <QWidget>
#include <QUrl>
#include <QMetaType>
#include <QMap>
#include <QModelIndex>

#include "regexp.h"
#include "regexppath.h"
#include "widget/regexpbaseinput.h"
#include "worker.h"
#include "searchparams.h"
#include "searchoptionswidget.h"
#include "searchhit.h"

#include "searchhits.h"

namespace Ui {
class SessionWidget;
}

class File;
class Worker;
class SearchBrowser;
class QTabWidget;
class AnchorClickHandler;
class SearchTab;

/*
Q_DECLARE_METATYPE(RegExp)
Q_DECLARE_METATYPE(RegExpPath)*/

class SessionWidget : public QWidget
{
    Q_OBJECT
    
public:

    explicit SessionWidget(QWidget *parent = nullptr);
    ~SessionWidget();

    void setCacheFileList(QAction* action);

    void cancelAll();

    QString path() const;

    void updateCompletions();

    void serialize(QJsonObject &json) const;
    void deserialize(const QJsonObject &v);
    SearchTab* find(int searchId);
    SearchTab *tab(int index);
    void countMatchedFiles();
    SearchTab *currentTab();
    int oldestTabIndex();
    SearchOptionsWidget *options() const;
    void setMode(Mode mode);


    void updateReplaceButton();

    void select();

    QString tabTitle(QString title, bool isExecuted) const;
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

    QAction* mCacheFileList;

    QStringList mFileList;

    void copyToNewTab();
    void updateTabText(int index);
    SearchTab *createTab();
signals:

    //void search(SearchParams);
    //void search(int action, int searchId, QString path, RegExpPath filter, bool notBinary, RegExp search, int linesBofore, int linesAfter, bool cacheFileList, QString);

    void search(SearchParams);

    void searchMore(int searchId);

    //void finishSearch(int searchId);
    //void collected();
    void collect();
    void replace(int);

    void canReplace(int);
    void getAllFiles(QString);

public slots:
    void onCanceled();

protected slots:

    void onCompleterActivated(QModelIndex);
    void onAllFiles(QString, QStringList);

    void onCanReplace(int,bool);

    void on_saveText_clicked();
    void on_saveHtml_clicked();
    void on_results_currentChanged(int index);
    //void onFound(int, QString, int, int, int, QString path);

    void onFound(int, SearchHits);

    //void onClone();
    void onSearch();
    void onPathChanged(QString path);
    void onPreview();
    void onReplace();
    void onReplaced(int, int, int, QStringList);
    void onPatternChanged(RegExp);
    void onFilterChanged(RegExpPath);
    void onReplacementChanged(RegExpReplacement value);
private:
    Ui::SessionWidget *ui;
    bool mListenOptions;
};

#endif // SESSIONWIDGET_H
