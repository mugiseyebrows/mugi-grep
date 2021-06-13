#ifndef NAVWIDGET_H
#define NAVWIDGET_H

#include <QWidget>

class QLineEdit;


namespace Ui {
class NavWidget;
}

class NavWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NavWidget(QWidget *parent = nullptr);
    ~NavWidget();

    QLineEdit* open();

private slots:
    void on_clear_clicked();

private:
    Ui::NavWidget *ui;
};

#endif // NAVWIDGET_H
