#include "oneormanyeditors.h"
#include "ui_oneormanyeditors.h"

OneOrManyEditors::OneOrManyEditors(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OneOrManyEditors)
{
    ui->setupUi(this);
}

OneOrManyEditors::~OneOrManyEditors()
{
    delete ui;
}

bool OneOrManyEditors::one() const
{
    return ui->oneEditor->isChecked();
}

bool OneOrManyEditors::many() const
{
    return !one();
}
