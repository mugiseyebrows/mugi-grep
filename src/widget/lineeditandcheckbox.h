#ifndef LINEEDITANDCHECKBOX_H
#define LINEEDITANDCHECKBOX_H

#include <QWidget>

namespace Ui {
class LineEditAndCheckbox;
}

class QCheckBox;
class QLineEdit;

class LineEditAndCheckbox : public QWidget
{
    Q_OBJECT

public:
    explicit LineEditAndCheckbox(QWidget *parent = nullptr);
    ~LineEditAndCheckbox();

    QCheckBox* checkBox();
    QLineEdit* lineEdit();

    QString text() const;
    void setText(const QString &text);
    bool isChecked() const;

signals:
    void textChanged(QString);

    //void clicked(bool);

private:
    Ui::LineEditAndCheckbox *ui;
};

#endif // LINEEDITANDCHECKBOX_H
