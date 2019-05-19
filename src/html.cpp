#include "html.h"



QString Html::span(const QString &text_, const QString &color, bool bold)
{
    QString text = text_;
    text.replace("<","&lt;").replace(">","&gt;");
    if (bold) {
        return QString("<span style=\"color:%1;font-weight: bold;\">%2</span>").arg(color).arg(text);
    }
    return QString("<span style=\"color:%1;\">%2</span>").arg(color).arg(text);
}

QString Html::span(const QString &text_, const QString &color, const QString &background) {
    QString text = text_;
    text.replace("<","&lt;").replace(">","&gt;");
    return QString("<span style=\"color:%2;background-color:%3;\">%1</span>").arg(text).arg(color).arg(background);
}

QString Html::anchor(const QString &text, const QString& path, const QString& color)
{
    return QString("<a href=\"%1\" style=\"color:%2; text-decoration: none\" >%3</a>").arg(path).arg(color).arg(text);
}

QString Html::spanZebra(const QStringList& text, const QString& color, const QString& background, const QString& altBackground)
{
    QStringList result;
    for(int i=0;i<text.size();i++) {
        result << span(text[i],color,i % 2 ? altBackground : background);
    }
    return result.join("");
}
