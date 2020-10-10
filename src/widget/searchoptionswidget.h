#ifndef SEARCHOPTIONSWIDGET_H
#define SEARCHOPTIONSWIDGET_H

#include <QWidget>
#include "regexppath.h"
#include "regexp.h"

class SearchBrowser;
class Worker;
class AnchorClickHandler;
class RXInput;
class RXPathInput;

namespace Ui {
class SearchOptionsWidget;
}

class SearchOptionsWidget : public QWidget
{
    Q_OBJECT

public:

    enum Mode {
        ModeSearch,
        ModeReplace
    };

    explicit SearchOptionsWidget(QWidget *parent = nullptr);
    ~SearchOptionsWidget();

    void collect();

    QString path() const;
    void setPath(const QString& path);

    void updateCompletions();

    bool validate();

    void setFiler(RegExpPath);
    void setPattern(RegExp);

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
    void setCacheFileList(QAction* action);
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

    QAction* mCacheFileList;

#endif

    void setMode(SearchOptionsWidget::Mode mode);

protected slots:
    //void on_doSearch_clicked();

signals:
    void patternChanged(RegExp);
    void filterChanged(RegExpPath);
    void search();

protected:
    Ui::SearchOptionsWidget *ui;
    bool mActive;
    Mode mMode;
};

#endif // SEARCHOPTIONSWIDGET_H
