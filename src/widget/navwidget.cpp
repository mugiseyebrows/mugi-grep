#include "navwidget.h"
#include "ui_navwidget.h"

NavWidget::NavWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavWidget)
{
    ui->setupUi(this);
}

NavWidget::~NavWidget()
{
    delete ui;
}

QLineEdit *NavWidget::open()
{
    return ui->open;
}

void NavWidget::on_clear_clicked()
{
    ui->open->clear();
}
