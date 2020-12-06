#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
class Settings;

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    enum class Mode {
        Undefined,
        One,
        Many
    };

    explicit SettingsDialog(Settings *settings, const QString& path, QWidget *parent = 0);
    ~SettingsDialog();

    void apply();

protected slots:
    void onButtonClicked(int buttonId, int row);
protected:

    Ui::SettingsDialog *ui;
    Settings* mSettings;
    Mode mMode;
    bool mApply;
};

#endif // SETTINGSDIALOG_H
