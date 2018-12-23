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

    //void setMode(RegExpBaseInput::Mode mode);

    void updateCollector();

    void serialize(QJsonObject &json) const;
    void deserialize(const QJsonObject &v);
protected:

    Worker* mWorker;
    QThread* mThread;

    static void save(const QString& path, const QString& text);
    int mSearchId;

    QMap<int,SearchBrowser*> mResults;
    //SearchBrowser* mSearchBrowser;

    QUrl mQueued;

    SearchBrowser* currentResult() const;

    bool mCancel;

    QTabWidget* mParent;

signals:

#if 0
    void cancel(int);
    void read(QString,RegExpPath,bool);
    void search(RegExp,RegExpPath,int,int);
    void searchMore(int);

#endif

    void setEditor();
    void search(int searchId, QString path, RegExpPath filter, bool notBinary, RegExp search, int linesBofore, int linesAfter,bool cacheFileList);
    void searchMore(int id);
    void finishSearch(int id);
    //void calcSize();
    void countMatchedFiles(QString,RegExpPath,bool);

protected slots:

    //void onStarted(int type, int id);
    //void onError(QString);
    //void onWalked(int);
    //void onRead(int,int,int);
    //void onCanceled(int);
    //void onFound(int id, QString, int, int, int);
    void onEditorSet();

    //void on_read_clicked();
    //void on_path_returnPressed();
    void on_selectPath_clicked();

    void on_search_clicked();
    void on_searchExp_returnPressed();
    void on_searchFilter_returnPressed();
    void on_linesAfter_returnPressed();
    void on_linesBefore_returnPressed();

    void onAnchorClicked(QUrl url);

    void on_cancel_clicked();

    void on_saveText_clicked();
    void on_saveHtml_clicked();

    //void on_readFilter_returnPressed();

    void on_results_currentChanged(int index);

    void onFound(int, QString, int, int, int, QString path);


    void on_path_textChanged(const QString &arg1);

    void onSearchFilterTextChanged();
    void onCountMatchedFiles(int, int);
private:
    Ui::SessionWidget *ui;
};

#endif // SESSIONWIDGET_H
