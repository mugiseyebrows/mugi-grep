#ifndef REGEXPINPUTPATH_H
#define REGEXPINPUTPATH_H

#include <QWidget>
#include <QMetaType>
#include <QStringList>

class QLineEdit;
class QCheckBox;
class QLabel;

#include "widget/regexpbaseinput.h"
#include "regexppath.h"

class RegExpPathInput : public RegExpBaseInput
{
    Q_OBJECT
public:

    explicit RegExpPathInput(QWidget *parent = 0);
    RegExpPath value() const;
    void setValue(const RegExpPath& value);

    void modeChanged();

protected:


signals:


public slots:
    
};

#endif // REGEXPINPUTPATH_H
