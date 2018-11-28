#include "widget/rxinput.h"
#include "ui_rxinput.h"
#include <QLineEdit>

RXInput::RXInput(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RXInput)
{
    ui->setupUi(this);

    mInputs << ui->include << ui->exclude;
    QComboBox* input;
    foreach(input,mInputs) {
        QObject::connect(input->lineEdit(),SIGNAL(returnPressed()),this,SIGNAL(returnPressed()));
    }
}

RXInput::~RXInput()
{
    delete ui;
}

RegExp RXInput::value() const {
    return RegExp(exps(),ui->matchCase->isChecked());
}

void RXInput::setValue(const RegExp &value) {
    setExps(value.exps());
    ui->matchCase->setChecked(value.case_());
}

