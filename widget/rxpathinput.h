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

    void setIncludePathValue(const QString& value);
    void setIncludeExtValue(const QString& value);
    void setExcludePathValue(const QString& value);
    void setExpludeExtValue(const QString& value);

protected slots:
    //void onTextChanged();

protected:
    bool mEmitTextChanged;

    Ui::RXPathInput *ui;
signals:
    void returnPressed();
    void textChanged();
    void caseClicked(bool);
};

#endif // RXPATHINPUT_H
