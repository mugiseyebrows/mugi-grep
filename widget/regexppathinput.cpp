#include "widget/regexppathinput.h"

#include <QLineEdit>

#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QDebug>

#include "utils/lit.h"

RegExpPathInput::RegExpPathInput(QWidget *parent) :
    RegExpBaseInput(4,Lit::sl("include","exts","exclude","exts"),0,parent)
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

RegExpPath RegExpPathInput::value() const
{
    return RegExpPath(exps(),mCase->isChecked());
}

void RegExpPathInput::setValue(const RegExpPath &value)
{
    setExps(value.exps());
    mCase->setChecked(value.case_());
}

void RegExpPathInput::modeChanged()
{
    for(int i=0;i<mInputs.size();i++) {
        mInputs[i]->setVisible(i == 1 || mMode == Expert);
        mLabels[i]->setVisible(mMode == Expert);
    }

}
