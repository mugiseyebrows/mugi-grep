// DO NOT EDIT ** This file was generated by mugicpp ** DO NOT EDIT
#include "replaceitem.h"

ReplaceItem::ReplaceItem() {
}
ReplaceItem::ReplaceItem(int line, const QString& before, const QString& after)
    : mLine(line), mBefore(before), mAfter(after) {
}
int ReplaceItem::line() const {
    return mLine;
}
void ReplaceItem::setLine(int value) {
    mLine = value;
}
QString ReplaceItem::before() const {
    return mBefore;
}
void ReplaceItem::setBefore(const QString& value) {
    mBefore = value;
}
QString ReplaceItem::after() const {
    return mAfter;
}
void ReplaceItem::setAfter(const QString& value) {
    mAfter = value;
}