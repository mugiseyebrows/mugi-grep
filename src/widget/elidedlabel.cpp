#include "elidedlabel.h"

#include <QStyle>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>

ElidedLabel::ElidedLabel(QWidget *parent) : QWidget(parent), mTextElideMode(Qt::ElideRight)
{

}

void ElidedLabel::paintEvent(QPaintEvent *)
{
    QStyle *style = QWidget::style();
    QPainter painter(this);

    int align = QStyle::visualAlignment(Qt::LeftToRight, QFlag(Qt::AlignLeft | Qt::AlignVCenter | Qt::TextExpandTabs));

    int flags = align | Qt::TextForceLeftToRight;

    QRect textRect = contentsRect();

    QString elided = this->fontMetrics().elidedText(mText, mTextElideMode, textRect.width());

    style->drawItemText(&painter, textRect, flags, this->palette(), isEnabled(), elided, foregroundRole());
}

QSize ElidedLabel::minimumSizeHint() const
{
    const QFontMetrics& fontMetrics = this->fontMetrics();
    QSize size = QSize(20,fontMetrics.lineSpacing());
    //qDebug() << "minimumSizeHint" << size;
    return size;
}

QSize ElidedLabel::sizeHint() const
{
    const QFontMetrics& fontMetrics = this->fontMetrics();
    QSize size = QSize(fontMetrics.horizontalAdvance(mText) ,fontMetrics.lineSpacing());
    //qDebug() << "sizeHint" << size;
    return size;
}

void ElidedLabel::setText(const QString &text)
{
    mText = text;
    update();
}

QString ElidedLabel::text() const
{
    return mText;
}

void ElidedLabel::setTextElideMode(Qt::TextElideMode mode)
{
    mTextElideMode = mode;
    update();
}

Qt::TextElideMode ElidedLabel::textElideMode() const
{
    return mTextElideMode;
}
