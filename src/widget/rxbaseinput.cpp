#include "widget/rxbaseinput.h"
#include <QLineEdit>

RXBaseInput::RXBaseInput() : mValidated(false)
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

bool RXBaseInput::validate(const QPalette& palette)
{
    QPalette paletteValid = palette;
    QPalette paletteInvalid = paletteValid;
    paletteInvalid.setColor(QPalette::Text,QColor(Qt::red));

    bool ok = true;
    foreach(QComboBox* input, mInputs) {
        QString text = input->lineEdit()->text();
        bool valid = text.isEmpty() || QRegularExpression(text).isValid();
        ok = ok && valid;
        input->setPalette(valid ? paletteValid : paletteInvalid);
    }
    mValidated = true;
    return ok;
}

void RXBaseInput::clearValidation(const QPalette &palette)
{
    if (!mValidated) {
        return;
    }
    foreach(QComboBox* input, mInputs) {
        input->setPalette(palette);
    }
    mValidated = false;
}

