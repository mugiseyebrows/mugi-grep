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

    QString path() const;

    void updateCollector();

    void serialize(QJsonObject &json) const;
    void deserialize(const QJsonObject &v);
    SearchBrowser* find(int searchId);
    SearchBrowser *tab(int index);
    void countMatchedFiles();
    SearchBrowser *currentTab();
    int oldestTabIndex();
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
signals:

    void search(int searchId, QString path, RegExpPath filter, bool notBinary, RegExp search, int linesBofore, int linesAfter,bool cacheFileList);
    void searchMore(int searchId);
    void finishSearch(int searchId);

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

private:
    Ui::SessionWidget *ui;
};

#endif // SESSIONWIDGET_H
