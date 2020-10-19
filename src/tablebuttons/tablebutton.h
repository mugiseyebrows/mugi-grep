#ifndef TABLEBUTTON_H
#define TABLEBUTTON_H
#include <QString>

class TableButtonImpl;
class QPushButton;
class QPoint;
class QSize;
class QIcon;
class QTableView;
class TableButton {
public:
    TableButton(TableButtonImpl* button = 0);
    TableButton& fixed(int index);
    TableButton& variable();
    TableButton& inside();
    TableButton& between();
    TableButton& insert();
    TableButton& remove();
    TableButton& append();
    TableButton& prepend();
    TableButton& horizontal();
    TableButton& vertical();
    TableButton& text(const QString& text);
    TableButton& icon(const QIcon& icon);
    TableButton& size(int w, int h);
    TableButton& size(const QSize& size);
    TableButton& iconSize(int w, int h);
    TableButton& iconSize(const QSize& size);
    TableButton& offset(int x, int y);
    TableButton& offset(const QPoint& offset);
    QPushButton* button() const;
    int id() const;
    bool valid() const;
    bool fixed() const;

protected:
    TableButtonImpl* mButton;
};
#endif // TABLEBUTTON_H
