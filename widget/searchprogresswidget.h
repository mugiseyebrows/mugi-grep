#ifndef SEARCHPROGRESSWIDGET_H
#define SEARCHPROGRESSWIDGET_H

#include <QWidget>

namespace Ui {
class SearchProgressWidget;
}

class SearchProgressWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearchProgressWidget(QWidget *parent = 0);
    ~SearchProgressWidget();

    void searchStarted();
    void init();
    void progress(int i, int t, int s, QString path);
    void aborted();
signals:
    void canceled();
private:
    Ui::SearchProgressWidget *ui;
};

#endif // SEARCHPROGRESSWIDGET_H
