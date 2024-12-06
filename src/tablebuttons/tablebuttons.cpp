#include "tablebuttons.h"
#include <QDebug>
#include "tablebuttongroup.h"
#include <QSet>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QDebug>
#include <QMouseEvent>
#include <QSignalMapper>
#include <QScrollBar>
#include "toset.h"

TableButtons::TableButtons(QObject *parent) : QObject(parent), mView(nullptr), mVisible(true)
{

}

TableButtons::~TableButtons()
{
    clear();
}

int TableButtons::nextId()
{
    QSet<int> keys = toSet(mButtons.keys());
    int i = 0;
    while (keys.contains(i)) {
        i++;
    }
    return i;
}

void TableButtons::create() {
    foreach(TableButtonImpl* button, mButtons) {
        if (!button->button()) {
            QPushButton* widget = button->create(mView);
            connect(widget,&QPushButton::clicked,[=](){
                emit clicked(button->id(), button->currentIndex());
            });
        }
    }
}

int verticalButtonsX(int viewportWidth, int shift, QHeaderView* horizontalHeader, int buttonsWidth) {
    int lastColumn = horizontalHeader->count() - 1;
    int x0 = horizontalHeader->sectionViewportPosition(lastColumn) + horizontalHeader->sectionSize(lastColumn) + shift;
    if (x0 + buttonsWidth > viewportWidth + shift) {
        x0 = viewportWidth + shift - buttonsWidth;
    }
    return x0;
}

int horizontalButtonsY(int viewportHeight, int shift, QHeaderView* verticalHeader, int buttonsHeight) {
    int lastColumn = verticalHeader->count() - 1;
    int y0 = verticalHeader->sectionViewportPosition(lastColumn) + verticalHeader->sectionSize(lastColumn) + shift;
    if (y0 + buttonsHeight > viewportHeight + shift) {
        y0 = viewportHeight + shift - buttonsHeight;
    }
    return y0;
}

int TableButtons::fixedButtonIndex(QHeaderView* header, TableButtonImpl* button) {
    int index = button->index();
    if (button->type() == TableButtonImpl::ButtonTypePrepend) {
        index = 0;
    } else if (button->type() == TableButtonImpl::ButtonTypeAppend) {
        index = header->count();
    }
    return index;
}

void TableButtons::clear()
{
    QList<TableButtonImpl*> values = mButtons.values();
    qDeleteAll(values);
    mButtons.clear();
}

TableButton TableButtons::button(int id)
{
    if (id < 0) {
        id = nextId();
    }

    if (!mButtons.contains(id)) {
        mButtons[id] = new TableButtonImpl(id);
    }

    return TableButton(mButtons[id]);
}

void TableButtons::setVisible(bool visible)
{
    mVisible = visible;
    update();
}

bool TableButtons::visible() const
{
    return mVisible;
}

void TableButtons::updateVertical() {

    TableButtonGroup buttons(mButtons.values(), Qt::Vertical);
    if (buttons.isEmpty()) {
        return;
    }
    QPoint shift_ = mView->viewport()->mapToParent(QPoint(0,0));
    QHeaderView* header = mView->verticalHeader();
    int x0 = verticalButtonsX(mView->viewport()->width(), shift_.x(), mView->horizontalHeader(), buttons.width());
    foreach(TableButtonImpl* button, buttons) {
        QPushButton* button_ = button->button();
        if (!mVisible) {
            button_->hide();
            continue;
        }
        int buttonSize = button_->height();
        int shift = shift_.y();
        int x = x0 + button->offset().x();
        int y = 0;
        if (button->type() == TableButtonImpl::ButtonTypeVariable) {
            int index;
            y = posBorderOrCenterShifted(button->position(), header, mPoint.y(), shift, buttonSize, &index);
            button->setCurrentIndex(index);
        } else if (button->type() == TableButtonImpl::ButtonTypeFixed ||
                   button->type() == TableButtonImpl::ButtonTypePrepend ||
                   button->type() == TableButtonImpl::ButtonTypeAppend) {
            int index = fixedButtonIndex(header, button);
            y = posBorderOrCenterFixedShifted(button->position(), header, index, shift, buttonSize);
            button->setCurrentIndex(index);
        }
        y += button->offset().y();
        button_->move(x,y);
        button_->show();
    }
}

int TableButtons::posBorderOrCenterShifted(TableButtonImpl::ButtonPosition position, QHeaderView* header,
                                           int pos, int shift, int buttonSize, int* index) {
    return position == TableButtonImpl::ButtonPositionInside ?
                posCenterShifted(header, pos, shift, buttonSize, index) :
                posBorderShifted(header, pos, shift, buttonSize, index);
}

int TableButtons::posCenterShifted(QHeaderView* header, int pos, int shift, int buttonSize, int* index) {
    return posCenter(header,pos,index) + shift - buttonSize / 2 - 1;
}

int TableButtons::posBorderShifted(QHeaderView* header, int pos, int shift, int buttonSize, int* index) {
    return posBorder(header,pos,index) + shift - buttonSize / 2 - 1;
}

int TableButtons::indexAt(QHeaderView* header, int pos) {
    int index = header->visualIndexAt(pos);
    if (index < 0) {
        index = header->count() - 1;
    }
    return index;
}

int TableButtons::posBorderOrCenterFixedShifted(TableButtonImpl::ButtonPosition position, QHeaderView* header, int index, int shift, int buttonSize) {
    return position == TableButtonImpl::ButtonPositionInside ? posCenterFixedShifted(header, index, shift, buttonSize) : posBorderFixedShifted(header, index, shift, buttonSize);
}

int TableButtons::posCenterFixedShifted(QHeaderView* header, int index, int shift, int buttonSize) {
    return posCenterFixed(header,index) + shift - buttonSize / 2 - 1;
}

int TableButtons::posBorderFixedShifted(QHeaderView* header, int index, int shift, int buttonSize) {
    return posBorderFixed(header,index) + shift - buttonSize / 2 - 1;
}

int TableButtons::posCenterFixed(QHeaderView* header, int index) {
    int sectionPos = header->sectionViewportPosition(index) + header->sectionSize(index) / 2;
    return sectionPos;
}

int TableButtons::posBorderFixed(QHeaderView* header, int index) {
    if (index >= header->count()) {
        return header->sectionViewportPosition(index-1) + header->sectionSize(index-1);
    }
    return header->sectionViewportPosition(index);
}

int TableButtons::posCenter(QHeaderView* header, int pos, int* index_) {
    int index = indexAt(header, pos);
    *index_ = index;
    int sectionPos = header->sectionViewportPosition(index) + header->sectionSize(index) / 2;
    return sectionPos;
}

int TableButtons::posBorder(QHeaderView* header, int pos, int* index_) {
    int index = indexAt(header, pos);
    int topPos = header->sectionViewportPosition(index);
    int bottomPos = header->sectionViewportPosition(index) + header->sectionSize(index);
    if (pos - topPos < bottomPos - pos) {
        *index_ = index;
        return topPos;
    } else {
        *index_ = index + 1;
        return bottomPos;
    }
}

void TableButtons::updateHorizontal() {

    TableButtonGroup buttons(mButtons.values(), Qt::Horizontal);
    if (buttons.isEmpty()) {
        return;
    }

    QPoint shift_ = mView->viewport()->mapToParent(QPoint(0,0));
    QHeaderView* header = mView->horizontalHeader();
    int y0 = horizontalButtonsY(mView->viewport()->height(), shift_.y(), mView->verticalHeader(), buttons.height());
    foreach(TableButtonImpl* button, buttons) {
        QPushButton* button_ = button->button();
        if (!mVisible) {
            button_->hide();
            continue;
        }
        int buttonSize = button_->width();
        int shift = shift_.x();
        int y = y0 + button->offset().y();
        int x = 0;
        if (button->type() == TableButtonImpl::ButtonTypeVariable) {
            int index;
            x = posBorderOrCenterShifted(button->position(), header, mPoint.x(), shift, buttonSize, &index);
            button->setCurrentIndex(index);
        } else if (button->type() == TableButtonImpl::ButtonTypeFixed ||
                   button->type() == TableButtonImpl::ButtonTypePrepend ||
                   button->type() == TableButtonImpl::ButtonTypeAppend) {
            int index = fixedButtonIndex(header, button);
            x = posBorderOrCenterFixedShifted(button->position(), header, index, shift, buttonSize);
            button->setCurrentIndex(index);
        }
        x += button->offset().x();
        button_->move(x,y);
        button_->show();
    }

}

void TableButtons::update()
{
    create();
    updateVertical();
    updateHorizontal();
}

void TableButtons::setView(QTableView *table)
{

    //mButton->setParent(table);
    //mButton->show();


    mView = table;
    /*foreach(b,mButtons) {
        b.button()->setParent(table);
    }*/

    //qDebug() << "scrollbars" << mView->horizontalScrollBar() << mView->verticalScrollBar();

    connect(mView->horizontalScrollBar(),SIGNAL(sliderMoved(int)),this,SLOT(onSliderMoved(int)));
    connect(mView->verticalScrollBar(),SIGNAL(sliderMoved(int)),this,SLOT(onSliderMoved(int)));

    table->setMouseTracking(true);
    table->viewport()->installEventFilter(this);
    connect(table->horizontalHeader(),SIGNAL(sectionResized(int,int,int)),this,SLOT(update()));
    connect(table->verticalHeader(),SIGNAL(sectionResized(int,int,int)),this,SLOT(update()));
    update();
}

bool TableButtons::eventFilter(QObject* object, QEvent* event)
{

    if (event->type() == QEvent::MouseMove && object == mView->viewport())
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (!mouseEvent)
            return false;
        mPoint = mouseEvent->pos();

        update();
    } else if (event->type() == QEvent::Resize && object == mView->viewport()) {
        update();
    }
    return false;
}


void TableButtons::onSliderMoved(int) {
    //qDebug() << "onSliderMoved";

    QPoint pos = mView->viewport()->mapFromGlobal(QCursor::pos());

    //qDebug() << "point (scrollbar)" << pos;

    mPoint = pos;

    update();
}
