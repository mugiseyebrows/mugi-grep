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
#include "replaceparams.h"
#include "searchhits.h"
#include "countfilesparams.h"
#include "getlistingparams.h"
#include "searchnamehits.h"
#include "renameparams.h"
#include "viewoptions.h"

namespace Ui {
class SessionWidget;
}

class File;
class Worker;
class SearchBrowser;
class QTabWidget;
class AnchorClickHandler;
class SearchTab;
class CallOnce;
class CountFilesManager;
class Settings;

#include "format.h"

class SessionWidget : public QWidget
{
    Q_OBJECT
    
public:

    explicit SessionWidget(Settings* settings, QWidget *parent = nullptr);
    ~SessionWidget();

    //void setCacheFileList(QAction* action);

    void cancelAll();

    QString path() const;

    void loadCollected();

    QJsonValue serialize() const;
    void deserialize(const QJsonValue &v);
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

    void save(Format format);

    void setViewOptions(const ViewOptions& options);

    ViewOptions viewOptions() const;
protected:

    Worker* mWorker;
    QThread* mThread;
    //CallOnce* mReplacementChanged;
    //CallOnce* mCountFiles;
    CallOnce* mGetListing;

    CountFilesManager* mCountFilesManager;

    bool mCancel;

    QSet<int> mCanceled;

    QTabWidget* mTabWidget;

    AnchorClickHandler* mClickHandler;

    SearchBrowser *createTab(const QString &name, SearchBrowser *browser = 0, bool append = true, bool select = true);

    void searchOrReplace(Worker::Action action);

    void copyToNewTab();
    void updateTabText(int index);
    SearchTab *createTab();

    QMap<QString,QStringList> mAllFiles;

    Settings* mSettings;

    int mQueedToRemove;


signals:

    //void search(SearchParams);
    //void search(int action, int searchId, QString path, RegExpPath filter, bool notBinary, RegExp search, int linesBofore, int linesAfter, bool cacheFileList, QString);

    void search(SearchParams);

    void searchMore(int searchId);

    //void finishSearch(int searchId);
    //void collected();
    void collect();
    void replace(ReplaceParams);
    void countFiles(CountFilesParams);

    void canReplace(int);
    void getListing(GetListingParams);

    void rename(RenameParams);

    void viewOptionsChanged(ViewOptions);

public slots:
    void onCanceled();
    void onCountFiles();
    void onGetListing();
    void onFilesCounted();
    void onRenamed(int, int);
    void onTabClose(int index);
protected slots:

    void onCompleterActivated(QModelIndex);
    void onListing(QString, QStringList);


    void on_results_currentChanged(int index);

    void onFound(int, SearchHits, SearchNameHits);

    //void onClone();
    void onSearch();
    void onPathChanged(QString path);
    void onPreview();
    void onReplace();
    //void onReplaced(int, int, int, QStringList);
    void onPatternChanged(RegExp);
    void onFilterChanged(RegExpPath);
    void onReplacementChanged(RegExpReplacement value);
    void onReplaced(ReplacedParams);
    void onCancel();

    void on_open_textChanged(QString);

    void onCacheFileListClicked(bool cacheFileList);

private:
    Ui::SessionWidget *ui;
    bool mListenOptions;
};

#endif // SESSIONWIDGET_H
