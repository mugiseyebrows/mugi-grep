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

protected:
    Ui::RXInput *ui;

signals:
    void returnPressed();
    void textChanged();
    void caseClicked(bool);

protected slots:
    void onClearValidation();
};

#endif // RXINPUT_H