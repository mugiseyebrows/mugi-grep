#include "displayoptionswidget.h"
#include "ui_displayoptionswidget.h"

DisplayOptionsWidget::DisplayOptionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DisplayOptionsWidget),
    mActive(true)
{
    ui->setupUi(this);
    connect(ui->fileName,SIGNAL(clicked()),this,SLOT(onChanged()));
    connect(ui->lineNumber,SIGNAL(clicked()),this,SLOT(onChanged()));
    connect(ui->wholeLine,SIGNAL(clicked()),this,SLOT(onChanged()));
    connect(ui->linesBefore,SIGNAL(valueChanged(int)),this,SLOT(onChanged()));
    connect(ui->linesAfter,SIGNAL(valueChanged(int)),this,SLOT(onChanged()));
    connect(ui->context,SIGNAL(clicked()),this,SLOT(onChanged()));
    connect(ui->signature,SIGNAL(clicked()),this,SLOT(onChanged()));
}

void DisplayOptionsWidget::on_reset_clicked() {
    setOptions(DisplayOptions());
}

void DisplayOptionsWidget::onChanged() {
    if (!mActive) {
        return;
    }
    emit optionsChanged();
}

DisplayOptionsWidget::~DisplayOptionsWidget()
{
    delete ui;
}

/*
int DisplayOptionsWidget::linesBefore() const
{
    return ui->linesBefore->value();
}

int DisplayOptionsWidget::linesAfter() const
{
    return ui->linesAfter->value();
}*/

DisplayOptions DisplayOptionsWidget::options() const
{
    return DisplayOptions(ui->linesBefore->value(),
                          ui->linesAfter->value(),
                          ui->fileName->isChecked(),
                          ui->lineNumber->isChecked(),
                          ui->wholeLine->isChecked(),
                          ui->context->isChecked(),
                          ui->signature->isChecked());
}

void DisplayOptionsWidget::setOptions(const DisplayOptions &options)
{
    mActive = false;
    ui->linesBefore->setValue(options.linesBefore());
    ui->linesAfter->setValue(options.linesAfter());
    ui->fileName->setChecked(options.fileName());
    ui->lineNumber->setChecked(options.lineNumber());
    ui->wholeLine->setChecked(options.wholeLine());
    ui->context->setChecked(options.context());
    ui->signature->setChecked(options.signature());
    mActive = true;
    emit optionsChanged();
}

void DisplayOptionsWidget::trigChanged()
{
    emit optionsChanged();
}
