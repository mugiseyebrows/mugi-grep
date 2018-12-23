#ifndef RXPATHINPUT_H
#define RXPATHINPUT_H

#include <QWidget>
#include "widget/rxbaseinput.h"
#include <QComboBox>
#include "regexppath.h"

namespace Ui {
class RXPathInput;
}

class RXPathInput : public QWidget, public RXBaseInput
{
    Q_OBJECT
    
public:
    explicit RXPathInput(QWidget *parent = 0);
    ~RXPathInput();

    RegExpPath value() const;
    void setValue(const RegExpPath &value);

    void enableTextChanged(bool active);

protected slots:
    void onTextChanged();

protected:
    bool mEmitTextChanged;

    Ui::RXPathInput *ui;
signals:
    void returnPressed();
    void textChanged();
};

#endif // RXPATHINPUT_H
