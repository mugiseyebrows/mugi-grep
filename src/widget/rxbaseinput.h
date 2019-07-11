#ifndef RXBASEINPUT_H
#define RXBASEINPUT_H

#include <QStringList>
#include <QComboBox>

class RXBaseInput
{
public:
    RXBaseInput();
    virtual QStringList exps() const;
    virtual void setExps(const QStringList &exps);
    QComboBox* input(int index);
    int count() const;
    virtual bool validate(const QPalette &palette);
    void clearValidation(const QPalette &palette);
protected:
    QList<QComboBox*> mInputs;
    bool mValidated;
};

#endif // RXBASEINPUT_H
