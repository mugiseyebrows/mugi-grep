#include "widget/rxpathinput.h"
#include "ui_rxpathinput.h"

#include "utils/sl.h"

#include <QLineEdit>

RXPathInput::RXPathInput(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RXPathInput)
{
    ui->setupUi(this);

    mInputs << ui->nameInclude
            << ui->extInclude
            << ui->nameExclude
            << ui->extExclude;
    QComboBox* input;
    foreach(input,mInputs) {
        QObject::connect(input->lineEdit(),SIGNAL(returnPressed()),this,SIGNAL(returnPressed()));
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

