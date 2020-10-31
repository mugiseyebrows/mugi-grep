#include "rxreplaceinput.h"
#include "ui_rxreplaceinput.h"

#include <QLineEdit>

RXReplaceInput::RXReplaceInput(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RXReplaceInput)
{
    ui->setupUi(this);
    mInputs << ui->replace;
    QComboBox* input;
    foreach(input,mInputs) {
        connect(input->lineEdit(),SIGNAL(returnPressed()),this,SIGNAL(returnPressed()));
        connect(input->lineEdit(),SIGNAL(textChanged(QString)),this,SLOT(onValueChanged()));
        connect(input->lineEdit(),SIGNAL(textChanged(QString)),this,SLOT(onClearValidation()));
    }
    connect(ui->preserveCase,SIGNAL(clicked(bool)),this,SLOT(onValueChanged()));
}

QWidgetList RXReplaceInput::widgets() const {
    return QWidgetList {ui->replace, ui->preserveCase};
}

RXReplaceInput::~RXReplaceInput()
{
    delete ui;
}

RegExpReplacement RXReplaceInput::value() const
{
    return RegExpReplacement(exps()[0], ui->preserveCase->isChecked());
}

void RXReplaceInput::setValue(const RegExpReplacement &value)
{
    QStringList exps;
    exps << value.pattern();
    setExps(exps);
    ui->preserveCase->setChecked(value.preserveCase());
}

bool RXReplaceInput::preserveCase() const
{
    return ui->preserveCase->isChecked();
}

void RXReplaceInput::setPreserveCase(bool value)
{
    ui->preserveCase->setChecked(value);
}

bool RXReplaceInput::validate(const QPalette &palette)
{
    return true;
}


void RXReplaceInput::onClearValidation() {
    RXBaseInput::clearValidation(palette());
}

void RXReplaceInput::onValueChanged() {
    emit valueChanged(value());
}
