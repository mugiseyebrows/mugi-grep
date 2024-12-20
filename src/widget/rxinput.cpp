#include "widget/rxinput.h"
#include "ui_rxinput.h"
#include <QLineEdit>
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
        connect(input->lineEdit(),SIGNAL(textChanged(QString)),this,SLOT(onValueChanged()));
        connect(input->lineEdit(),SIGNAL(textChanged(QString)),this,SLOT(onClearValidation()));
        input->setMaximumWidth(maximumWidth);
    }
    connect(ui->matchCase,SIGNAL(clicked(bool)),this,SLOT(onValueChanged()));
}

QWidgetList RXInput::widgets() const {
    return QWidgetList {ui->include, ui->exclude, ui->matchCase};
}

void RXInput::setChildVisible(bool exclude, bool matchCase)
{
    ui->exclude->setVisible(exclude);
    ui->label->setVisible(exclude);
    ui->matchCase->setVisible(matchCase);
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

#if 0
void RXInput::onTextChanged() {
    emit textChanged(value());
}
#endif
void RXInput::onValueChanged() {
    emit valueChanged(value());
}
