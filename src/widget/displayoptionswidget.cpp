#include "displayoptionswidget.h"
#include "ui_displayoptionswidget.h"

DisplayOptionsWidget::DisplayOptionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DisplayOptionsWidget)
{
    ui->setupUi(this);
    connect(ui->filename,SIGNAL(clicked()),this,SIGNAL(optionsChanged()));
    connect(ui->lineNumber,SIGNAL(clicked()),this,SIGNAL(optionsChanged()));
    connect(ui->wholeLine,SIGNAL(clicked()),this,SIGNAL(optionsChanged()));
    connect(ui->linesBefore,SIGNAL(valueChanged(int)),this,SIGNAL(optionsChanged()));
    connect(ui->linesAfter,SIGNAL(valueChanged(int)),this,SIGNAL(optionsChanged()));
}

DisplayOptionsWidget::~DisplayOptionsWidget()
{
    delete ui;
}

int DisplayOptionsWidget::linesBefore() const
{
    return ui->linesBefore->value();
}

int DisplayOptionsWidget::linesAfter() const
{
    return ui->linesAfter->value();
}
