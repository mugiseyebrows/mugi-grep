#include "html.h"

QString Html::span(const QString &text_, const QString &color)
{
    QString text = text_;
    text.replace("<","&lt;").replace(">","&gt;");
    return QString("<span style=\"color:%1\">%2</span>").arg(color).arg(text);
}

QString Html::anchor(const QString &text, const QString& path, const QString& color)
{
    return QString("<a href=\"%1\" style=\"color:%2; text-decoration: none\" >%3</a>").arg(path).arg(color).arg(text);
}
