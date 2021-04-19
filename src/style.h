#ifndef STYLE_H
#define STYLE_H

#include <QProxyStyle>

class Style : public QProxyStyle
{
    Q_OBJECT
public:
    explicit Style(QStyle *style = nullptr);

protected:
    QStyle* mWindows;

public:
    void drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const;
};

#endif // STYLE_H
