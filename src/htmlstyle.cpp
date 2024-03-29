#include "htmlstyle.h"

HtmlStyle::HtmlStyle() : mFontWeight(FontWeight::Undefined),
    mTextDecoration(TextDecoration::Undefined), mWhiteSpace(WhiteSpace::Undefined) {

}

HtmlStyle &HtmlStyle::color(const QString &color) {
    mColor = color;
    return *this;
}

HtmlStyle &HtmlStyle::backgroundColor(const QString &color) {
    mBackgroundColor = color;
    return *this;
}

HtmlStyle &HtmlStyle::fontWeight(HtmlStyle::FontWeight value) {
    mFontWeight = value;
    return *this;
}

HtmlStyle &HtmlStyle::whiteSpace(HtmlStyle::WhiteSpace whiteSpace)
{
    mWhiteSpace = whiteSpace;
    return *this;
}

HtmlStyle &HtmlStyle::textDecoration(HtmlStyle::TextDecoration value) {
    mTextDecoration = value;
    return *this;
}

QString HtmlStyle::toString() const {

    QStringList props;
    if (!mColor.isEmpty()) {
        props.append(QString("color: %1").arg(mColor));
    }
    if (!mBackgroundColor.isEmpty()) {
        props.append(QString("background-color: %1").arg(mBackgroundColor));
    }
    if (mFontWeight != FontWeight::Undefined) {
        static QMap<FontWeight, QString> m = {{FontWeight::Normal, "normal"}, {FontWeight::Bold,"bold"}};
        props.append(QString("font-weight: %1").arg(m[mFontWeight]));
    }
    if (mTextDecoration != TextDecoration::Undefined) {
        static QMap<TextDecoration, QString> m = {{TextDecoration::None, "none"}};
        props.append(QString("text-decoration: %1").arg(m[mTextDecoration]));
    }
    if (mWhiteSpace != WhiteSpace::Undefined) {
        static QMap<WhiteSpace, QString> m = {{WhiteSpace::PreWrap, "pre-wrap"}};
        props.append(QString("white-space: %1").arg(m[mWhiteSpace]));
    }

    if (props.isEmpty()) {
        return QString();
    }

    return QString("style=\"%1\"").arg(props.join("; "));
}
