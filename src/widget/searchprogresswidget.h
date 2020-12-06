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

    void started();
    void init();
    void progress(int i, int t, int s, QString path);
    void aborted();
    void replaced(int files, int lines);
    void renamed(int successful, int failed);
signals:
    void canceled();
private:
    Ui::SearchProgressWidget *ui;
};

#endif // SEARCHPROGRESSWIDGET_H
