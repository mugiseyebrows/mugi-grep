#ifndef REGEXPINPUT_H
#define REGEXPINPUT_H

#include <QWidget>
#include <QMetaType>

#include "regexp.h"
#include "widget/regexpbaseinput.h"

class QLineEdit;
class QCheckBox;

class RegExpInput : public RegExpBaseInput
{
    Q_OBJECT
public:
    explicit RegExpInput(QWidget *parent = 0);
    
    RegExp value() const;
    void setValue(const RegExp& value);

protected:

    void modeChanged();

public slots:
    
};

#endif // REGEXPINPUT_H
