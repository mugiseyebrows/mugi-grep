#include "tablebutton.h"
#include "tablebuttonimpl.h"
#include <QIcon>
#include <QPoint>
#include <QPushButton>
#include <QSize>
#include <QTableView>

TableButton::TableButton(TableButtonImpl* button) : mButton(button) {
}

TableButton& TableButton::fixed(int index) {
    mButton->fixed(index);
    return *this;
}

TableButton& TableButton::variable() {
    mButton->variable();
    return *this;
}

TableButton& TableButton::inside() {
    mButton->inside();
    return *this;
}

TableButton& TableButton::between() {
    mButton->between();
    return *this;
}

TableButton& TableButton::insert() {
    mButton->insert();
    return *this;
}

TableButton& TableButton::remove() {
    mButton->remove();
    return *this;
}

TableButton& TableButton::append() {
    mButton->append();
    return *this;
}

TableButton& TableButton::prepend() {
    mButton->prepend();
    return *this;
}

TableButton& TableButton::horizontal() {
    mButton->horizontal();
    return *this;
}

TableButton& TableButton::vertical() {
    mButton->vertical();
    return *this;
}

TableButton& TableButton::text(const QString& text) {
    mButton->text(text);
    return *this;
}

TableButton& TableButton::icon(const QIcon& icon) {
    mButton->icon(icon);
    return *this;
}

TableButton& TableButton::size(int w, int h) {
    mButton->size(w, h);
    return *this;
}

TableButton& TableButton::size(const QSize& size) {
    mButton->size(size);
    return *this;
}

TableButton& TableButton::iconSize(int w, int h) {
    mButton->iconSize(w, h);
    return *this;
}

TableButton& TableButton::iconSize(const QSize& size) {
    mButton->iconSize(size);
    return *this;
}

TableButton& TableButton::offset(int x, int y) {
    mButton->offset(x, y);
    return *this;
}

TableButton& TableButton::offset(const QPoint& offset) {
    mButton->offset(offset);
    return *this;
}

QPushButton* TableButton::button() const {
    return mButton->button();
}

int TableButton::id() const {
    return mButton->id();
}

bool TableButton::valid() const {
    return mButton != 0;
}

bool TableButton::fixed() const {
    return mButton->type() == TableButtonImpl::ButtonTypeFixed;
}
