#ifndef HTML_H
#define HTML_H

#include <QStringList>
class HtmlStyle;

class Html
{
public:

    static QString span(const QString &text_, const QString &color, bool bold = false);

    static QString span(const QString &text_, const QString &color, const QString &background);

    static QString anchor(const QString &text, const QString& path, const QString& color);

    static QString spanZebra(const QStringList& text, const QString& color, const QString& background, const QString& altBackground);

    static QString span(const QString &text_, const HtmlStyle& style);

    static QString br();

};

#endif // HTML_H
