#include "htmldivs.h"

#include "htmlstyle.h"

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
