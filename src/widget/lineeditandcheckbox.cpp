#include "lineeditandcheckbox.h"
#include "ui_lineeditandcheckbox.h"

#include <QLineEdit>
#include <QCheckBox>

LineEditAndCheckbox::LineEditAndCheckbox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LineEditAndCheckbox)
{
    ui->setupUi(this);

    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(textChanged(QString)));

}

LineEditAndCheckbox::~LineEditAndCheckbox()
{
    delete ui;
}

QCheckBox *LineEditAndCheckbox::checkBox()
{
    return ui->checkBox;
}

QLineEdit *LineEditAndCheckbox::lineEdit()
{
    return ui->lineEdit;
}

QString LineEditAndCheckbox::text() const {
    return ui->lineEdit->text();
}

void LineEditAndCheckbox::setText(const QString& text) {
    ui->lineEdit->setText(text);
}

bool LineEditAndCheckbox::isChecked() const {
    return ui->checkBox->isChecked();
}


