#include "widget/settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QStandardItemModel>
#include "settings.h"

#include "model/editorsmodel.h"

#include "tablebuttons/tablebuttons.h"

#include "editorsdialog.h"
#include "oneormanyeditors.h"

SettingsDialog::SettingsDialog(const QString &path, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    mMode(Mode::Undefined),
    mApply(false)
{
    ui->setupUi(this);

    EditorsModel* model = new EditorsModel(this);
    Settings::instance()->toModel(model);

    if (model->isEmpty()) {
        OneOrManyEditors dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            if (dialog.one()) {
                mMode = Mode::One;
            } else {
                mMode = Mode::Many;
            }
        }
    }

    ui->editors->setModel(model);

    TableButtons* buttons = new TableButtons();
    buttons->setView(ui->editors);
    buttons->button(0).vertical().text("set");

    connect(buttons,SIGNAL(clicked(int,int)),this,SLOT(onButtonClicked(int,int)));

    if (mMode != Mode::Undefined) {
        mApply = true;
        int row = 0;
        if (mMode == Mode::One) {
            row = model->append(QString());
        } else if (mMode == Mode::Many) {
            row = model->append(path);
        }
        onButtonClicked(0, row);
    } else {
        if (path.isEmpty()) {
            return;
        }
        mApply = true;
        int row = model->append(path);
        onButtonClicked(0, row);
    }

}

#include <QTimer>


void SettingsDialog::onButtonClicked(int buttonId, int row) {

    if (buttonId != 0) {
        return;
    }

    EditorsDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    Editor editor = dialog.editor();
    if (editor.isNull()) {
        return;
    }
    QAbstractItemModel* model = ui->editors->model();
    model->setData(model->index(row, 1), editor.app());
    if (mApply) {
        apply();
        QTimer::singleShot(0,this,SLOT(accept()));
    }
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
