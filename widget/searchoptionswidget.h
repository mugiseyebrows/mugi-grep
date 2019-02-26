#ifndef SEARCHOPTIONSWIDGET_H
#define SEARCHOPTIONSWIDGET_H

#include <QWidget>

class SearchBrowser;

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

protected:


signals:
    void search();
    void clone();
protected slots:
    void onFilterTextChanged();
    void onExpTextChanged();
    void on_selectPath_clicked();
    void on_search_clicked();

    void onLinesAfterValueChanged();
    void onLinesBeforeValueChanged();
    void onCacheFileListClicked(bool);
protected:
    Ui::SearchOptionsWidget *ui;
    bool mActive;
    SearchBrowser* mBrowser;
};

#endif // SEARCHOPTIONSWIDGET_H
