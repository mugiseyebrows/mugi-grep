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

    enum class WhiteSpace {
        Undefined,
        PreWrap,
    };

    HtmlStyle();

    HtmlStyle& color(const QString& color);
    HtmlStyle& backgroundColor(const QString& color);
    HtmlStyle& fontWeight(FontWeight value);

    HtmlStyle& whiteSpace(WhiteSpace whiteSpace);

    HtmlStyle& textDecoration(TextDecoration value);

    QString toString() const;

    QString mColor;
    QString mBackgroundColor;
    FontWeight mFontWeight;
    TextDecoration mTextDecoration;
    WhiteSpace mWhiteSpace;

};


#endif // HTMLSTYLE_H
