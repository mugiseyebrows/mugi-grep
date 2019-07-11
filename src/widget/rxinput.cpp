#include "widget/rxinput.h"
#include "ui_rxinput.h"
#include <QLineEdit>
#include <QDesktopWidget>
#include <QScreen>
#include <QDebug>

RXInput::RXInput(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RXInput)
{
    ui->setupUi(this);

    QRect rect = qApp->primaryScreen()->geometry();
    int maximumWidth = rect.width() / 2;

    mInputs << ui->include << ui->exclude;
    QComboBox* input;
    foreach(input,mInputs) {
        connect(input->lineEdit(),SIGNAL(returnPressed()),this,SIGNAL(returnPressed()));
        connect(input->lineEdit(),SIGNAL(textChanged(QString)),this,SIGNAL(textChanged()));
        connect(input->lineEdit(),SIGNAL(textChanged(QString)),this,SLOT(onClearValidation()));
        input->setMaximumWidth(maximumWidth);
    }
    connect(ui->matchCase,SIGNAL(clicked(bool)),this,SIGNAL(caseClicked(bool)));
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

void RXInput::onClearValidation() {
    RXBaseInput::clearValidation(palette());
}
