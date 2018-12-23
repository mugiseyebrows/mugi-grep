#include "widget/rxpathinput.h"
#include "ui_rxpathinput.h"

#include "utils/sl.h"

#include <QLineEdit>
#include <QDebug>

RXPathInput::RXPathInput(QWidget *parent) :
    QWidget(parent),
    mEmitTextChanged(true),
    ui(new Ui::RXPathInput)
{
    ui->setupUi(this);

    mInputs << ui->nameInclude
            << ui->extInclude
            << ui->nameExclude
            << ui->extExclude;
    QComboBox* input;
    foreach(input,mInputs) {
        connect(input->lineEdit(),SIGNAL(returnPressed()),this,SIGNAL(returnPressed()));
        connect(input->lineEdit(),SIGNAL(textChanged(QString)),this,SLOT(onTextChanged()));
    }

}

RXPathInput::~RXPathInput()
{
    delete ui;
}

RegExpPath RXPathInput::value() const {
    return RegExpPath(exps(),ui->matchCase->isChecked());
}

void RXPathInput::setValue(const RegExpPath &value) {
    setExps(value.exps());
    ui->matchCase->setChecked(value.case_());
}

void RXPathInput::enableTextChanged(bool active)
{
    mEmitTextChanged = active;
}

void RXPathInput::setIncludePathValue(const QString &value)
{
    ui->nameInclude->lineEdit()->setText(value);
}

void RXPathInput::setIncludeExtValue(const QString &value)
{
    ui->extInclude->lineEdit()->setText(value);
}

void RXPathInput::setExcludePathValue(const QString &value)
{
    ui->nameExclude->lineEdit()->setText(value);
}

void RXPathInput::setExpludeExtValue(const QString &value)
{
    ui->extExclude->lineEdit()->setText(value);
}

void RXPathInput::onTextChanged()
{
    if (!mEmitTextChanged) {
        //qDebug() << "!mEmitTextChanged";
        return;
    }
    //qDebug() << "emit textChanged()";
    emit textChanged();
}

