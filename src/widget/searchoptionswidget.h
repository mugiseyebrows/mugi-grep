#ifndef SEARCHOPTIONSWIDGET_H
#define SEARCHOPTIONSWIDGET_H

#include <QWidget>
#include "regexppath.h"

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

    void setActive(bool active);
    void setBrowser(SearchBrowser* browser, bool setValues = true);
    void setMode(Mode mode);

    QString path() const;
    void setPath(const QString& path);

    void collect();
    void emitTabTitle();
    void init(Worker *worker, AnchorClickHandler *clickHandler);
    void setBrowserValues();
    void updateCompletions();
    void countMatchedFiles();

    void setCanReplace(bool can);

    void select();
    void setCacheFileList(QAction* action);
signals:
    void search();
    void preview();
    void replace();
    void clone();
    void tabTitle(QString, bool);
    void countMatchedFiles(QString,RegExpPath,bool);
    void pathChanged(QString);
protected slots:
    void onFilterTextChanged();
    void onExpTextChanged();
    void onLinesAfterValueChanged();
    void onLinesBeforeValueChanged();
    void onCountMatchedFiles(int matched, int total);
    void onNotBinaryClicked(bool value);
    void onReplacementTextChanged(QString path);
    void on_selectPath_clicked();
    void on_search_clicked();
    void on_path_textChanged(QString path);
    void on_replace_clicked();
    void on_preview_clicked();

protected:
    Ui::SearchOptionsWidget *ui;
    bool mActive;
    SearchBrowser* mBrowser;
    Worker* mWorker;
    AnchorClickHandler* mClickHandler;
    Mode mMode;
    QAction* mCacheFileList;
};

#endif // SEARCHOPTIONSWIDGET_H
