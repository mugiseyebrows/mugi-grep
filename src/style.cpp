#include "style.h"

#include <QStyleFactory>

Style::Style(QStyle *style) : QProxyStyle(style), mWindows(0)
{
    QStringList keys = QStyleFactory::keys();
    if (keys.contains("Windows")) {
        mWindows = QStyleFactory::create("Windows");
    }
}

void Style::drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const
{
    if (mWindows) {
        if (pe == QStyle::PE_IndicatorTabClose) {
            return mWindows->drawPrimitive(pe,opt,p,w);
        }
    }
    return baseStyle()->drawPrimitive(pe, opt, p, w);
}
