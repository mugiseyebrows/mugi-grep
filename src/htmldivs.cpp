#include "htmldivs.h"

#include "htmlstyle.h"

QString HtmlDivs::div(const QString &text, const QString &backgroundColor)
{
    HtmlDivs divs;
    divs.append(text, backgroundColor);
    divs.close();
    return divs.divs().join("");
}

void HtmlDivs::append(const QString &line, const QString &backgoundColor) {
    if (!mBackgroundColor.isEmpty() && mBackgroundColor != backgoundColor) {
        close();
    }
    mLines.append(line);
    mBackgroundColor = backgoundColor;
}

void HtmlDivs::close() {
    if (mLines.isEmpty()) {
        return;
    }
    HtmlStyle style;
    style.backgroundColor(mBackgroundColor);
    mDivs << QString("<div %1>").arg(style.toString()) + mLines.join("<br/>") + "</div>";
    mLines.clear();
}

QStringList HtmlDivs::divs() const {
    return mDivs;
}

bool HtmlDivs::isEmpty() const
{
    return mLines.isEmpty() && mDivs.isEmpty();
}

void HtmlDivs::clear()
{
    mLines.clear();
    mDivs.clear();
    mBackgroundColor = QString();
}

