#include "widget/settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QStandardItemModel>
#include "settings.h"

#include "model/editorsmodel.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    EditorsModel* model = new EditorsModel(this);
    Settings::instance()->toModel(model);
    ui->editors->setModel(model);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::apply()
{
    QAbstractItemModel* model = ui->editors->model();
    Settings::instance()->fromModel(model);
    Settings::instance()->save();
}
