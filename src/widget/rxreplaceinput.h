#ifndef RXREPLACEINPUT_H
#define RXREPLACEINPUT_H

#include <QWidget>

#include "rxbaseinput.h"

namespace Ui {
class RXReplaceInput;
}

class RXReplaceInput : public QWidget, public RXBaseInput
{
    Q_OBJECT

public:
    explicit RXReplaceInput(QWidget *parent = 0);
    ~RXReplaceInput();

    QString value() const;
    void setValue(const QString& value);

    bool validate(const QPalette &palette) override;

signals:
    void returnPressed();
    void textChanged(QString);

protected slots:
    void onClearValidation();
private:
    Ui::RXReplaceInput *ui;
};

#endif // RXREPLACEINPUT_H
