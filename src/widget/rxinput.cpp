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
    connect(ui->matchCase, &QCheckBox::clicked, this, &RXInput::onValueChanged);
    connect(ui->multiline, &QCheckBox::clicked, this, &RXInput::onValueChanged);
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

RegExpPair RXInput::value() const {
    bool caseSensitive = ui->matchCase->isChecked();
    bool multiline = ui->multiline->isChecked();
    bool dotAll = multiline;
    return RegExpPair(ui->include->currentText(), ui->exclude->currentText(), caseSensitive, dotAll, multiline);
}

void RXInput::setValue(const RegExpPair &value) {
    //setExps(value.exps());
    ui->include->setCurrentText(value.include());
    ui->exclude->setCurrentText(value.exclude());
    ui->matchCase->setChecked(value.case_());
    ui->multiline->setChecked(value.multiline());
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
