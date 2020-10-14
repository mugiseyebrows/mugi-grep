#include "widget/rxpathinput.h"
#include "ui_rxpathinput.h"

#include <QLineEdit>
#include <QDebug>
#include <QScreen>

RXPathInput::RXPathInput(QWidget *parent) :
    QWidget(parent),
    mEmitTextChanged(true),
    ui(new Ui::RXPathInput)
{
    ui->setupUi(this);

    QRect rect = qApp->primaryScreen()->geometry();
    int maximumWidth = rect.width() / 4;

    mInputs << ui->nameInclude
            << ui->extInclude
            << ui->nameExclude
            << ui->extExclude;
    QComboBox* input;
    foreach(input,mInputs) {
        connect(input->lineEdit(),SIGNAL(returnPressed()),this,SIGNAL(returnPressed()));
        connect(input->lineEdit(),SIGNAL(textChanged(QString)),this,SLOT(onValueChanged()));
        connect(input->lineEdit(),SIGNAL(textChanged(QString)),this,SLOT(onClearValidation()));
        input->setMaximumWidth(maximumWidth);
    }

    connect(ui->matchCase,SIGNAL(clicked(bool)),this,SLOT(onValueChanged()));
    connect(ui->notBinary,SIGNAL(clicked(bool)),this,SLOT(onValueChanged()));
}

RXPathInput::~RXPathInput()
{
    delete ui;
}

RegExpPath RXPathInput::value() const {
    return RegExpPath(exps(),ui->matchCase->isChecked(),ui->notBinary->isChecked());
}

QCheckBox* RXPathInput::notBinary() {
    return ui->notBinary;
}

void RXPathInput::setValue(const RegExpPath &value) {
    setExps(value.patterns());
    ui->matchCase->setChecked(value.case_());
    ui->notBinary->setChecked(value.notBinary());
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

QCheckBox *RXPathInput::matchCaseCheckBox() const
{
    return ui->matchCase;
}

void RXPathInput::onClearValidation() {
    RXBaseInput::clearValidation(palette());
}

void RXPathInput::onValueChanged() {
    emit valueChanged(value());
}

#if 0
void RXPathInput::onTextChanged() {
    //emit textChanged(value());
    emit valueChanged(value());
}
#endif
