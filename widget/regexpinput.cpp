#include "regexpinput.h"

#include <QLineEdit>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLabel>

#include "utils/sl.h"

RegExpInput::RegExpInput(QWidget *parent) :
    RegExpBaseInput(2,sl("include","exclude"),nullptr,parent)
{
    QList<QWidget*> widgets;
    for(int i=0;i<mInputs.size();i++) {
        widgets << mLabels[i] << mInputs[i];
    }
    widgets << mCase;
    QWidget* widget;
    foreach(widget,widgets) {
        layout()->addWidget(widget);
    }
}

RegExp RegExpInput::value() const
{
    if (mMode == Simple) {
        RegExp(mInputs[0]->text(),"",mCase->isChecked());
    }
    return RegExp(mInputs[0]->text(),mInputs[1]->text(),mCase->isChecked());
}

void RegExpInput::setValue(const RegExp &value)
{
    mInputs[0]->setText(value.include());
    mInputs[1]->setText(value.exclude());
    mCase->setChecked(value.case_());
}

void RegExpInput::modeChanged()
{
    for(int i=0;i<mInputs.size();i++) {
        mInputs[i]->setVisible(i == 0 || mMode == Expert);
        mLabels[i]->setVisible(mMode == Expert);
    }
}
