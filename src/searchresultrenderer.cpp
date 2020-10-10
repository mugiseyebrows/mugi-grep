#include "searchresultrenderer.h"

#include "displayoptionswidget.h"
#include <QTextBrowser>
#include <QDir>
#include "searchtab.h"

SearchResultRenderer::SearchResultRenderer(QObject *parent) : QObject(parent)
{

}

void SearchResultRenderer::setTab(SearchTab *tab)
{
    mTab = tab;
    connect(tab->displayOptions(),SIGNAL(optionsChanged()),this,SLOT(onOptionsChanged()));
}

QStringList SearchResultRenderer::toHtmlSpans(const ColoredLine& coloredLine,
                                   const QStringList& backgroundColors) {
    QStringList cols;
    QList<ColoredLineSpan> spans = coloredLine.spans();
    foreach (const ColoredLineSpan& span, spans) {
        QString col = coloredLine.string().mid(span.start(), span.length());
        if (span.background() == 0 || span.background() >= backgroundColors.size()) {
            cols << Html::span(col, span.foreground() == 0 ? "black" : "red");
        } else {
            cols << Html::span(col, span.foreground() == 0 ? "black" : "red",
                               backgroundColors[span.background()]);
        }
    }
    return cols;
}
QStringList SearchResultRenderer::fileNameLineNumber(bool showFileName, bool showLineNumber,
                                          const QString& relativePath, const QString& href,
                                          int lineNumber) {
    QStringList cols;
    if (showFileName) {
        cols << Html::anchor(relativePath, href, "violet") << Html::span(":", "blue");
    }
    if (showLineNumber) {
        cols << Html::span(QString::number(lineNumber), "green") << Html::span(":", "blue");
    }
    return cols;
}

void SearchResultRenderer::append(const SearchHits& hits) {

    //SearchHits& hits_ = mTab->hits();

    RegExp pattern = hits.pattern();

    int before = 0;
    int after = 0;

    //hits.read(before, after);

    QStringList res;

    for (int j = 0; j < hits.size(); j++) {

        SearchHit hit = hits.hit(j);
        QSet<int> siblings = hit.siblings(before, after);
        QList<int> matched = hit.hits();

        static QStringList backgroundColors = {"#ffffff", "#ffdfba", "#baffc9",
                                               "#bae1ff", "#ffffba", "#ffb3ba"};

        bool onlyMatched = false;
        bool showFileName = true;
        bool showLineNumber = true;

        int max1 = *std::max_element(matched.begin(), matched.end());
        int max2 = siblings.isEmpty() ? 0 : *std::max_element(siblings.begin(), siblings.end());
        int max_ = qMax(max1, max2);

        QMap<int, QString> lines = hit.cache();

        QString mPath = hit.path();
        QString mRelativePath = hit.relativePath();

        for (int i = 0; i <= max_; i++) {

            if (matched.contains(i)) {

                QString line = lines[i];

                QRegularExpression regexp = pattern.includeExp();
                QRegularExpressionMatchIterator it = regexp.globalMatch(line);

                QString href = "file:///" + QDir::toNativeSeparators(mPath) +
                               "?line=" + QString::number(i + 1);

                if (onlyMatched) {
                    // each match on new line
                    while (it.hasNext()) {
                        QStringList cols = fileNameLineNumber(showFileName, showLineNumber,
                                                              mRelativePath, href, i + 1);
                        QRegularExpressionMatch m = it.next();
                        ColoredLine coloredLine(line);
                        int jmax = qMin(m.lastCapturedIndex(), backgroundColors.size() - 1);
                        for (int j = 1; j <= jmax; j++) {
                            coloredLine.paintBackground(m.capturedStart(j), m.capturedEnd(j), j);
                        }
                        coloredLine.paintForeground(m.capturedStart(), m.capturedEnd(), 1);
                        cols << toHtmlSpans(coloredLine.mid(m.capturedStart(), m.capturedLength()),
                                            backgroundColors);
                        res << cols.join("");
                    }
                } else {
                    // all matches on one line
                    QStringList cols = fileNameLineNumber(showFileName, showLineNumber,
                                                          mRelativePath, href, i + 1);
                    ColoredLine coloredLine(line);
                    while (it.hasNext()) {
                        QRegularExpressionMatch m = it.next();
                        int jmax = qMin(m.lastCapturedIndex(), backgroundColors.size() - 1);
                        for (int j = 1; j <= jmax; j++) {
                            coloredLine.paintBackground(m.capturedStart(j), m.capturedEnd(j), j);
                        }
                        coloredLine.paintForeground(m.capturedStart(), m.capturedEnd(), 1);
                    }
                    cols << toHtmlSpans(coloredLine, backgroundColors);
                    res << cols.join("");
                }

            } else if (siblings.contains(i)) {

                QString line = lines[i];
                QString href = "file:///" + QDir::toNativeSeparators(mPath) +
                               "?line=" + QString::number(i + 1);
                QStringList cols;
                if (showFileName) {
                    cols << Html::anchor(mRelativePath, href, "violet") << Html::span("-", "blue");
                }
                if (showLineNumber) {
                    cols << Html::span(QString::number(i + 1), "green") << Html::span("-", "blue");
                }
                cols << Html::span(line, "black");
                res << cols.join("");
            }
        }
    }
    mTab->textBrowser()->append(res.join("<br/>"));
}

void SearchResultRenderer::onOptionsChanged() {
    int linesBefore = mTab->displayOptions()->linesBefore();
    int linesAfter = mTab->displayOptions()->linesAfter();
    mTab->hits().read(linesBefore, linesAfter);
    mTab->textBrowser()->clear();
    this->append(mTab->hits());
}
