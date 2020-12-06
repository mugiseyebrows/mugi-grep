#include "tablebuttonimpl.h"

TableButtonImpl::TableButtonImpl(int id)
    : mId(id), mButton(nullptr), mOrientation(Qt::Vertical), mType(ButtonTypeVariable),
      mPosition(ButtonPositionInside), mIndex(-1) {
}

TableButtonImpl::~TableButtonImpl()
{
    if (mButton) {
        mButton->setParent(0);
        mButton->deleteLater();
        mButton = 0;
    }
}

void TableButtonImpl::fixed(int index) {
    mType = ButtonTypeFixed;
    mIndex = index;
}

void TableButtonImpl::variable() {
    mType = ButtonTypeVariable;
}

void TableButtonImpl::inside() {
    mPosition = ButtonPositionInside;
}

void TableButtonImpl::between() {
    mPosition = ButtonPositionBetween;
}

void TableButtonImpl::insert() {
    mType = ButtonTypeVariable;
    mPosition = ButtonPositionBetween;
}

void TableButtonImpl::remove() {
    mType = ButtonTypeVariable;
    mPosition = ButtonPositionInside;
}

void TableButtonImpl::append() {
    mType = ButtonTypeAppend;
    mPosition = ButtonPositionBetween;
}

void TableButtonImpl::prepend() {
    mType = ButtonTypePrepend;
    mPosition = ButtonPositionBetween;
}

void TableButtonImpl::horizontal() {
    mOrientation = Qt::Horizontal;
}

void TableButtonImpl::vertical() {
    mOrientation = Qt::Vertical;
}

void TableButtonImpl::text(const QString& text) {
    mText = text;
    update(Text);
}

void TableButtonImpl::icon(const QIcon& icon) {
    mIcon = icon;update(Icon);
}

void TableButtonImpl::size(int w, int h) {
    mSize = QSize(w, h);update(Size);
}

void TableButtonImpl::size(const QSize& size) {
    mSize = size;update(Size);
}

void TableButtonImpl::iconSize(int w, int h) {
    mIconSize = QSize(w, h);update(IconSize);
}

void TableButtonImpl::iconSize(const QSize& size) {
    mIconSize = size;
update(IconSize);
}

void TableButtonImpl::offset(int x, int y) {
    mOffset = QPoint(x, y);
}

void TableButtonImpl::offset(const QPoint& offset) {
    mOffset = offset;
}

QPushButton* TableButtonImpl::button() const {
    return mButton;
}

int TableButtonImpl::id() const {
    return mId;
}

QPoint TableButtonImpl::offset() const {
    return mOffset;
}

QSize TableButtonImpl::size() const {
    return mSize;
}

QSize TableButtonImpl::iconSize() const {
    return mIconSize;
}

Qt::Orientation TableButtonImpl::orientation() const {
    return mOrientation;
}

QIcon TableButtonImpl::icon() const {
    return mIcon;
}

int TableButtonImpl::index() const {
    return mIndex;
}

TableButtonImpl::ButtonType TableButtonImpl::type() const {
    return mType;
}

TableButtonImpl::ButtonPosition TableButtonImpl::position() const {
    return mPosition;
}

QString TableButtonImpl::text() const {
    return mText;
}

int TableButtonImpl::top() const {
    return mOffset.y();
}

int TableButtonImpl::left() const {
    return mOffset.x();
}

int TableButtonImpl::bottom() const {
    QSize size = mSize.isEmpty() ? mButton->size() : mSize;
    return size.height() + top();
}

int TableButtonImpl::right() const {
    QSize size = mSize.isEmpty() ? mButton->size() : mSize;
    return size.width() + left();
}

int TableButtonImpl::currentIndex() const {
    return mCurrentIndex;
}

void TableButtonImpl::setCurrentIndex(int currentIndex) {
    mCurrentIndex = currentIndex;
}

QPushButton* TableButtonImpl::create(QTableView* view) {
    if (mButton) {
        return mButton;
    }
    mButton = new QPushButton(mText, view);
    update(All);
    return mButton;
}

void TableButtonImpl::update(int updateProp)
{
    if (!mButton) {
        return;
    }
    if (!mSize.isEmpty() && updateProp & Size) {
        mButton->setFixedSize(mSize);
    }
    if (!mIcon.isNull()) {
        if (updateProp && updateProp & Icon) {
            mButton->setIcon(mIcon);
        }
        if (!mIconSize.isNull() && updateProp & IconSize) {
            mButton->setIconSize(mIconSize);
        }
    }
    if (!mText.isEmpty() && updateProp & Text) {
        mButton->setText(mText);
    }
}
