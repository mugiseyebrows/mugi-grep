#ifndef RXREPLACEINPUT_H
#define RXREPLACEINPUT_H

#include <QWidget>

#include "rxbaseinput.h"
#include "regexpreplacement.h"

namespace Ui {
class RXReplaceInput;
}

class RXReplaceInput : public QWidget, public RXBaseInput
{
    Q_OBJECT

public:
    explicit RXReplaceInput(QWidget *parent = 0);
    ~RXReplaceInput();

    RegExpReplacement value() const;
    void setValue(const RegExpReplacement& value);
    bool preserveCase() const;
    void setPreserveCase(bool value);

    bool validate(const QPalette &palette) override;

    QWidgetList widgets() const;
signals:
    void returnPressed();
    void valueChanged(RegExpReplacement);

protected slots:
    void onClearValidation();
    void onValueChanged();
private:
    Ui::RXReplaceInput *ui;
};

#endif // RXREPLACEINPUT_H
