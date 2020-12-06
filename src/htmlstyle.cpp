#include "htmlstyle.h"

HtmlStyle::HtmlStyle() : mFontWeight(FontWeight::Undefined), mTextDecoration(TextDecoration::Undefined) {

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

HtmlStyle &HtmlStyle::textDecoration(HtmlStyle::TextDecoration value) {
    mTextDecoration = value;
    return *this;
}

QString HtmlStyle::toString() const {
    if (mColor.isEmpty() && mBackgroundColor.isEmpty() && mFontWeight == FontWeight::Undefined) {
        return QString();
    }
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
    return QString("style=\"%1\"").arg(props.join("; "));
}
