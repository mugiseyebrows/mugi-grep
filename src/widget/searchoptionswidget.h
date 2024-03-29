#ifndef SEARCHOPTIONSWIDGET_H
#define SEARCHOPTIONSWIDGET_H

#include <QWidget>
#include "regexppath.h"
#include "regexp.h"
#include "regexpreplacement.h"
#include "mode.h"
#include "viewoptions.h"

class SearchBrowser;
class Worker;
class AnchorClickHandler;
class RXInput;
class RXPathInput;

class QCheckBox;
class QLineEdit;

namespace Ui {
class SearchOptionsWidget;
}

class SearchOptionsWidget : public QWidget
{
    Q_OBJECT

public:

    explicit SearchOptionsWidget(QWidget *parent = nullptr);
    ~SearchOptionsWidget();

    void collect(Mode mode);

    QString path() const;
    void setPath(const QString& path);

    void loadCollected();

    bool validate();

    RegExpPath filter() const;

    void setFiler(const RegExpPath &);
    void setPattern(const RegExp &);
    void setReplacement(const RegExpReplacement &value);

    QLineEdit* pathEdit() const;

    //void setCacheFileList(QAction* action);

    void setReplaceEnabled(bool enabled);

    void setViewOptions(const ViewOptions& options);

#if 0
    void setActive(bool active);
    void setBrowser(SearchBrowser* browser, bool setValues = true);
    void setMode(Mode mode);






    //void emitTabTitle();
    void init(Worker *worker, AnchorClickHandler *clickHandler);
    //void setBrowserValues();

    void countMatchedFiles();

    void setCanReplace(bool can);

    void select();

signals:
    void search();
    void preview();
    void replace();
    void clone();
    //void tabTitle(QString, bool);
    void countMatchedFiles(QString,RegExpPath,bool);
    void pathChanged(QString);
protected slots:
    //void onFilterTextChanged();
    //void onExpTextChanged();
    //void onLinesAfterValueChanged();
    //void onLinesBeforeValueChanged();
    void onCountMatchedFiles(int matched, int total);
    void onNotBinaryClicked(bool value);
    void onPreserveCaseClicked(bool value);
    void onReplacementTextChanged(QString path);
    void on_selectPath_clicked();
    void on_search_clicked();
    void on_path_textChanged(QString path);
    void on_replace_clicked();
    void on_preview_clicked();
    void on_showFileName_clicked(bool checked);
    void on_showLineNumber_clicked(bool checked);
    void on_onlyMatched_clicked(bool checked);
    void onCacheToggled(bool checked);
protected:


    SearchBrowser* mBrowser;
    Worker* mWorker;
    AnchorClickHandler* mClickHandler;



#endif

    void showFileCount(int filtered, int total);

    void hideFileCount();

    void setMode(Mode mode);

    bool renameFiles() const;

    void setPreviewEnabled(bool enabled);

    bool cacheFileListChecked() const;

    void setCacheFileListChecked(bool value);

    void fixLayout();
    //bool notBinary() const;
    QCheckBox *cacheFileList();
    QWidgetList widgets();
    ViewOptions viewOptions() const;
public slots:
    void on_select_clicked();
protected slots:
    //void on_doSearch_clicked();

signals:
    void patternChanged(RegExp);
    void filterChanged(RegExpPath);
    void replacementChanged(RegExpReplacement);
    void pathChanged(QString);
    void notBinaryChanged(bool);

    void search();
    void preview();
    void replace();
    //void select();

protected:
    Ui::SearchOptionsWidget *ui;
    //bool mActive;
    Mode mMode;
    ViewOptions mViewOptions;

};

#endif // SEARCHOPTIONSWIDGET_H
