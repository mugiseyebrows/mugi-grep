#ifndef DISPLAYOPTIONSWIDGET_H
#define DISPLAYOPTIONSWIDGET_H

#include <QWidget>

namespace Ui {
class DisplayOptionsWidget;
}

class DisplayOptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DisplayOptionsWidget(QWidget *parent = nullptr);
    ~DisplayOptionsWidget();

    int linesBefore() const;
    int linesAfter() const;

signals:
    void optionsChanged();

private:
    Ui::DisplayOptionsWidget *ui;
};

#endif // DISPLAYOPTIONSWIDGET_H
