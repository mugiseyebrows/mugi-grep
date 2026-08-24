#ifndef STATWIDGET_H
#define STATWIDGET_H

#include <QWidget>

namespace Ui {
class StatWidget;
}

class StatWidget : public QWidget
{
    Q_OBJECT

public:
    enum Button {
        Include,
        Exclude
    };

    explicit StatWidget(const QList<QPair<QString, qint64> > &stat, QWidget *parent = nullptr);

    ~StatWidget();

protected:
    QStringList checked() const;

signals:
    void clicked(int, QString);

protected slots:
    void onInclude();
    void onExclude();
private:
    Ui::StatWidget *ui;
};

#endif // STATWIDGET_H
