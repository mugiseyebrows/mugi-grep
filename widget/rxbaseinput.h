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
protected:
    QList<QComboBox*> mInputs;
};

#endif // RXBASEINPUT_H
