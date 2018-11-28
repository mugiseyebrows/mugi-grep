#include "widget/rxbaseinput.h"
#include <QLineEdit>

RXBaseInput::RXBaseInput()
{
}

QStringList RXBaseInput::exps() const
{
    QStringList res;
    QComboBox* input;
    foreach(input,mInputs) {
        res << input->currentText();
    }
    return res;
}

void RXBaseInput::setExps(const QStringList &exps)
{
    for(int i=0;i<mInputs.size();i++) {
        QComboBox* input = mInputs[i];
        input->lineEdit()->setText(exps.value(i));
    }
}

QComboBox *RXBaseInput::input(int index)
{
    return mInputs[index];
}

int RXBaseInput::count() const
{
    return mInputs.size();
}
