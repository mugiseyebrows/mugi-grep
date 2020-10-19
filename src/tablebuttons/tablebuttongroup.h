#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include "tablebuttonimpl.h"

class TableButtonGroup : public QList<TableButtonImpl*>
{
public:
    TableButtonGroup(const QList<TableButtonImpl*>& buttons, Qt::Orientation orientation);

    int left() const;
    int right() const;
    int top() const;
    int bottom() const;

    int width() const;
    int height() const;
};

#endif // BUTTONGROUP_H
