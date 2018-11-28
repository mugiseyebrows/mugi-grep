#include "widget/regexpbaseinput.h"

#include <QHBoxLayout>

#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

RegExpBaseInput::RegExpBaseInput(int inputs, const QStringList& labels, QCheckBox* case_, QWidget *parent) :
    QWidget(parent), mMode(Expert)
{
    for (int i=0;i<inputs;i++) {
        mInputs << new QLineEdit();
    }
    QString label;
    foreach(label,labels) {
        mLabels << new QLabel(label);
    }

    if (case_ != nullptr) {
        mCase = case_;
    } else {
        mCase = new QCheckBox("case");
        QHBoxLayout* layout = new QHBoxLayout();
        layout->setMargin(0);
        this->setLayout(layout);
    }

    for(int i=0;i<mInputs.size();i++) {
        connect(mInputs[i],SIGNAL(returnPressed()),this,SIGNAL(returnPressed()));
    }
}

void RegExpBaseInput::setMode(Mode mode)
{
    mMode = mode;
    modeChanged();
}

RegExpBaseInput::Mode RegExpBaseInput::mode() const
{
    return mMode;
}

QStringList RegExpBaseInput::exps() const {
    QStringList res;
    QLineEdit* edit;
    foreach(edit,mInputs) {
        res << edit->text();
    }
    return res;
}

void RegExpBaseInput::setExps(const QStringList &exps)
{
    for(int i=0;i<mInputs.size();i++) {
        mInputs[i]->setText(exps[i]);
    }
}
