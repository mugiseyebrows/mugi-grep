#ifndef SEARCHOPTIONSWIDGET_H
#define SEARCHOPTIONSWIDGET_H

#include <QWidget>
#include "regexppath.h"

class SearchBrowser;
class Worker;
class AnchorClickHandler;

namespace Ui {
class SearchOptionsWidget;
}

class SearchOptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearchOptionsWidget(QWidget *parent = nullptr);
    ~SearchOptionsWidget();

    void setActive(bool active);
    void setBrowser(SearchBrowser* browser, bool setValues = true);

    QString path() const;
    void setPath(const QString& path);

    void collect();
    void emitTabTitle();
    void init(Worker *worker, AnchorClickHandler *clickHandler);
    void setBrowserValues();
    void updateCollector();
    void countMatchedFiles();

signals:
    void search();
    void clone();
    void tabTitle(QString, bool);
    void countMatchedFiles(QString,RegExpPath,bool);
    void pathChanged(QString);
protected slots:
    void onFilterTextChanged();
    void onExpTextChanged();
    void on_selectPath_clicked();
    void on_search_clicked();
    void onLinesAfterValueChanged();
    void onLinesBeforeValueChanged();
    void onCacheFileListClicked(bool);
    void onCountMatchedFiles(int matched, int total);
    void onNotBinaryClicked(bool value);
protected:
    Ui::SearchOptionsWidget *ui;
    bool mActive;
    SearchBrowser* mBrowser;
    Worker* mWorker;
    AnchorClickHandler* mClickHandler;
private slots:
    void on_selectFiles_clicked();
    void on_path_textChanged(const QString &arg1);
};

#endif // SEARCHOPTIONSWIDGET_H
