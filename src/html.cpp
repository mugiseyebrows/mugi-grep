#include "html.h"

#include "htmlstyle.h"

QString Html::span(const QString &text_, const QString &color, bool bold)
{
    QString text = text_;
    text.replace("<","&lt;").replace(">","&gt;");

    HtmlStyle style;
    style.color(color).fontWeight(bold ? HtmlStyle::FontWeight::Bold : HtmlStyle::FontWeight::Undefined);
    return QString("<span %1>%2</span>").arg(style.toString()).arg(text);
}

QString Html::span(const QString &text_, const QString &color, const QString &background) {
    QString text = text_;
    text.replace("<","&lt;").replace(">","&gt;");

    HtmlStyle style;
    style.color(color).backgroundColor(background);
    return QString("<span %1>%2</span>").arg(style.toString()).arg(text);
}

QString Html::span(const QString &text_, const HtmlStyle& style) {
    QString text = text_;
    text.replace("<","&lt;").replace(">","&gt;");
    return QString("<span %1>%2</span>").arg(style.toString()).arg(text);
}

QString Html::anchor(const QString &text, const QString& path, const QString& color)
{
    HtmlStyle style;
    style.color(color).textDecoration(HtmlStyle::TextDecoration::None);
    return QString("<a href=\"%1\" %2 >%3</a>").arg(path).arg(style.toString()).arg(text);
}

QString Html::spanZebra(const QStringList& text, const QString& color, const QString& background, const QString& altBackground)
{
    QStringList result;
    for(int i=0;i<text.size();i++) {
        result << span(text[i],color,i % 2 ? altBackground : background);
    }
    return result.join("");
}
