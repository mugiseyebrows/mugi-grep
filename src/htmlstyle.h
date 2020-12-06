#ifndef HTMLSTYLE_H
#define HTMLSTYLE_H

#include <QString>
#include <QColor>
#include <QMap>

class HtmlStyle {
public:
    enum class FontWeight {
        Undefined,
        Normal,
        Bold
    };
    enum TextDecoration {
        Undefined,
        None
    };

    HtmlStyle();

    HtmlStyle& color(const QString& color);
    HtmlStyle& backgroundColor(const QString& color);
    HtmlStyle& fontWeight(FontWeight value);

    HtmlStyle& textDecoration(TextDecoration value);

    QString toString() const;

    QString mColor;
    QString mBackgroundColor;
    FontWeight mFontWeight;
    TextDecoration mTextDecoration;
};


#endif // HTMLSTYLE_H
