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
        connect(input->lineEdit(),SIGNAL(textChanged(QString)),this,SIGNAL(textChanged(QString)));
        connect(input->lineEdit(),SIGNAL(textChanged(QString)),this,SLOT(onClearValidation()));
    }
    connect(ui->preserveCase,SIGNAL(clicked(bool)),this,SIGNAL(preserveCaseClicked(bool)));
}

RXReplaceInput::~RXReplaceInput()
{
    delete ui;
}

QString RXReplaceInput::value() const
{
    QStringList exps = this->exps();
    return exps[0];
}

void RXReplaceInput::setValue(const QString &value)
{
    QStringList exps;
    exps << value;
    setExps(exps);
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
