#ifndef INTLINEEDIT_H
#define INTLINEEDIT_H

#include <QLineEdit>

class IntLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
public:
    explicit IntLineEdit(QWidget *parent = 0);

    int value() const;

    void setValue(int value);
    
signals:
    
    void valueChanged(int);

public slots:
    
    void onTextChanged(QString);

};

#endif // INTLINEEDIT_H
