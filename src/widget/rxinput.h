#ifndef RXINPUT_H
#define RXINPUT_H

#include <QWidget>
#include "regexp.h"

namespace Ui {
class RXInput;
}
#include <QComboBox>

#include "widget/rxbaseinput.h"

class RXInput : public QWidget, public RXBaseInput
{
    Q_OBJECT
    
public:
    explicit RXInput(QWidget *parent = 0);
    ~RXInput();

    RegExp value() const;
    void setValue(const RegExp &value);

    QWidgetList widgets() const;
protected:
    Ui::RXInput *ui;

signals:
    void returnPressed();
    //void textChanged(RegExp);
    //void caseClicked(bool);
    void valueChanged(RegExp);

protected slots:
    void onClearValidation();
    //void onTextChanged();
    void onValueChanged();
};

#endif // RXINPUT_H
