#include "divcontext.h"

#include "htmlstyle.h"
#include "divhit.h"
#include "html.h"

DivContext::DivContext(const QString absolutePath, const QString relativePath, int lineNumber, const QString &context)
    : absolutePath(absolutePath), relativePath(relativePath), lineNumber(lineNumber), context(context) {

}

QString DivContext::render(const Colors &, bool showFileName, bool showLineNumber) const {
    HtmlStyle style;
    if (!backgroundColor.isEmpty()) {
        style.backgroundColor(backgroundColor);
    }
    style.color(color);
    QStringList items = fileNameLineNumberContext(color, showFileName,
                                                  showLineNumber, relativePath,
                                                  fileHref(absolutePath, lineNumber), lineNumber + 1);
    HtmlStyle bold;
    bold.color(color);//.fontWeight(HtmlStyle::FontWeight::Bold);
    items.append(Html::span(context, bold));
    return QString("<div %1>%2</div>").arg(style.toString()).arg(items.join(""));
}
