#ifndef DISPLAYOPTIONSWIDGET_H
#define DISPLAYOPTIONSWIDGET_H

#include <QWidget>

#include "displayoptions.h"

namespace Ui {
class DisplayOptionsWidget;
}

class DisplayOptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DisplayOptionsWidget(QWidget *parent = nullptr);
    ~DisplayOptionsWidget();

    /*int linesBefore() const;
    int linesAfter() const;*/

    DisplayOptions options() const;
    void setOptions(const DisplayOptions& options);

    void trigChanged();

signals:
    void optionsChanged();

protected:
    Ui::DisplayOptionsWidget *ui;
    bool mActive;
protected slots:
    void onChanged();
    void on_reset_clicked();
};

#endif // DISPLAYOPTIONSWIDGET_H
