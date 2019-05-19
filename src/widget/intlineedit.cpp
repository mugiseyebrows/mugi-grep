#include "widget/intlineedit.h"

IntLineEdit::IntLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    connect(this,SIGNAL(textChanged(QString)),this,SLOT(onTextChanged(QString)));
}

int IntLineEdit::value() const
{
    return text().toInt();
}

void IntLineEdit::setValue(int value)
{
    setText(QString::number(value));
}

void IntLineEdit::onTextChanged(QString val)
{
    if (val.isEmpty())
        return;

    bool ok;
    int val_ = val.toInt(&ok);
    if (!ok)
        return;

    emit valueChanged(val_);
}
