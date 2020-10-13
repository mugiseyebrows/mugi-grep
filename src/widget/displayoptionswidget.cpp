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
                          ui->wholeLine->isChecked());
}

void DisplayOptionsWidget::setOptions(const DisplayOptions &options)
{
    mActive = false;
    ui->linesBefore->setValue(options.linesBefore());
    ui->linesAfter->setValue(options.linesAfter());
    ui->fileName->setChecked(options.fileName());
    ui->lineNumber->setChecked(options.lineNumber());
    ui->wholeLine->setChecked(options.wholeLine());
    mActive = true;
    emit optionsChanged();
}

void DisplayOptionsWidget::trigChanged()
{
    emit optionsChanged();
}
