#include "divhit.h"

#include "htmlstyle.h"
#include "html.h"
#include "colors.h"
#include <QDir>

QString DivHit::render(const Colors &colors, bool showFileName, bool showLineNumber) const {
    QStringList items;
    for(int i=0;i<lines.size();i++) {
        const LineHit& line = lines[i];
        int lineNumber = line.lineNumber;
        QString separator = line.matched ? ":" : "-";
        QStringList cols = fileNameLineNumber(colors, showFileName, showLineNumber, relativePath, fileHref(absolutePath, lineNumber), lineNumber + 1, separator);
        cols += line.spans;
        if (i+1 < lines.size()) {
            cols.append("\n");
        }
        items.append(cols.join(""));
    }
    HtmlStyle style;
    style.backgroundColor(backgroundColor).whiteSpace(HtmlStyle::WhiteSpace::PreWrap);

    QString style_ = style.toString();

    return QString("<div %1>%2</div>").arg(style_).arg(items.join(""));
}

QStringList fileNameLineNumber(const Colors &colors, bool showFileName, bool showLineNumber, const QString &relativePath, const QString &href, int lineNumber, const QString &separator) {
    QStringList cols;
    if (showFileName) {
        cols << Html::anchor(relativePath, href, colors.anchorColor()) << Html::span(separator, colors.separatorColor());
    }
    if (showLineNumber) {
        cols << Html::span(QString::number(lineNumber), colors.linenumberColor()) << Html::span(separator, colors.separatorColor());
    }
    return cols;
}



QStringList fileNameLineNumberContext(const QString &color, bool showFileName, bool showLineNumber, const QString &relativePath, const QString &href, int lineNumber) {
    QStringList cols;
    QString separator = ":";
    if (showFileName) {
        cols << Html::anchor(relativePath, href, color) << Html::span(separator, color);
        //cols << Html::anchor(relativePath, href, backgroundColor) << Html::span(separator, backgroundColor);
    }
    if (showLineNumber) {
        cols << Html::span(QString::number(lineNumber), color) << Html::span(separator, color);
    }
    return cols;
}

QString fileHref(const QString &path, int lineNumber) {
    return "file:///" + QDir::toNativeSeparators(path) +
            "?line=" + QString::number(lineNumber + 1);
}

QString DivHit2::render(const Colors &colors, bool showFileName, bool showLineNumber) const {

    QString style_ = "";

    QStringList lines;
    for(int i=0;i<mSpans.size();i++) {
        lines.append(QString("%1:%2").arg(i).arg(mSpans[i].join("")));
    }
    return QString("<div %1>%2</div>").arg(style_).arg(lines.join(Html::br()));
}

/*
void DivHit2::setSpans(const QStringList &spans) {
    mSpans = spans;
}*/
