#ifndef RXINPUT_H
#define RXINPUT_H

#include <QWidget>
#include "regexppair.h"

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

    RegExpPair value() const;
    void setValue(const RegExpPair &value);

    QWidgetList widgets() const;

    void setChildVisible(bool exclude, bool matchCase);

protected:
    Ui::RXInput *ui;

signals:
    void returnPressed();
    //void textChanged(RegExpPair);
    //void caseClicked(bool);
    void valueChanged(RegExpPair);

protected slots:
    void onClearValidation();
    //void onTextChanged();
    void onValueChanged();
};

#endif // RXINPUT_H
