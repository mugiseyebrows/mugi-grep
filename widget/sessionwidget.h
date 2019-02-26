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

namespace Ui {
class SessionWidget;
}

class File;
class Worker;
class SearchBrowser;
class QTabWidget;
class AnchorClickHandler;

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

    QString readPath() const;

    void startRead();

    void updateCollector();

    void serialize(QJsonObject &json) const;
    void deserialize(const QJsonObject &v);
    SearchBrowser* find(int searchId);
    SearchBrowser *tab(int index);
    void countMatchedFiles();
    SearchBrowser *currentTab();

protected:

    Worker* mWorker;
    QThread* mThread;

    static void save(const QString& path, const QString& text);
    //int mSearchId;

    //QMap<int,SearchBrowser*> mResults;
    SearchBrowser* currentResult() const;

    bool mCancel;

    QTabWidget* mParent;

    AnchorClickHandler* mClickHandler;

    SearchBrowser *createTab(const QString &name, SearchBrowser *browser = 0, bool append = true, bool select = true);

    bool mListenResultCurrentChanged;
signals:

    void setEditor();
    void search(int searchId, QString path, RegExpPath filter, bool notBinary, RegExp search, int linesBofore, int linesAfter,bool cacheFileList);
    void searchMore(int id);
    void finishSearch(int id);
    void countMatchedFiles(QString,RegExpPath,bool);

protected slots:

#if 0
    void on_selectPath_clicked();
    void on_search_clicked();
    void on_searchExp_returnPressed();
    void on_searchFilter_returnPressed();
    void on_linesAfter_returnPressed();
    void on_linesBefore_returnPressed();
#endif

    void on_cancel_clicked();

    void on_saveText_clicked();
    void on_saveHtml_clicked();

    void on_results_currentChanged(int index);

    void onFound(int, QString, int, int, int, QString path);

    //void on_path_textChanged(const QString &arg1);

    //void onSearchFilterTextChanged();
    void onCountMatchedFiles(int, int);

    //void on_selectFiles_clicked();

    //void onSearchExpTextChanged();
private:
    Ui::SessionWidget *ui;
};

#endif // SESSIONWIDGET_H
