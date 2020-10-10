#include "searchresultrenderer.h"

#include "displayoptionswidget.h"
#include <QTextBrowser>
#include <QDir>
#include "searchtab.h"
#include <QDebug>

SearchResultRenderer::SearchResultRenderer(QObject *parent) : QObject(parent), mZebra(false)
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

QMap<int, bool> SearchResultRenderer::doZebra(int before, int after, const QList<int>& matched) {
    QMap<int, bool> result;
    for(int j=0;j<matched.size();j++) {
        int match = matched[j];
        for(int i=match-before;i<match+after+1;i++) {
            result[i] = j % 2 ? mZebra : !mZebra;
        }
    }
    if (matched.size() % 2) {
        mZebra = !mZebra;
    }
    return result;
}

void SearchResultRenderer::append(const SearchHits& hits) {

    //SearchHits& hits_ = mTab->hits();

    RegExp pattern = hits.pattern();

    int before = mTab->displayOptions()->linesBefore();
    int after = mTab->displayOptions()->linesAfter();

    //hits.read(before, after);

    QStringList res;

    for (int j = 0; j < hits.size(); j++) {

        SearchHit hit = hits.hit(j);
        QSet<int> siblings = hit.siblings(before, after);
        QList<int> matched = hit.hits();

        QMap<int, bool> zebra = doZebra(before, after, matched);

        static QStringList backgroundColors = {"#ffffff", "#ffdfba", "#baffc9",
                                               "#bae1ff", "#ffffba", "#ffb3ba"};

        static QStringList grayZebraColors = {"#E6E6E6", "#F0F0F0"};

        int zebraColor1 = backgroundColors.size();
        int zebraColor2 = zebraColor1 + 1;

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

                //qDebug() << pattern.includeExp().pattern();

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
                                            backgroundColors + grayZebraColors);
                        res << cols.join("");
                    }
                } else {
                    // all matches on one line
                    QStringList cols = fileNameLineNumber(showFileName, showLineNumber,
                                                          mRelativePath, href, i + 1);
                    ColoredLine coloredLine(line);

                    if (before > 0 || after > 0) {
                        coloredLine.paintBackground(zebra[i] ? zebraColor1 : zebraColor2);
                    }

                    while (it.hasNext()) {
                        QRegularExpressionMatch m = it.next();
                        int jmax = qMin(m.lastCapturedIndex(), backgroundColors.size() - 1);
                        for (int j = 1; j <= jmax; j++) {
                            coloredLine.paintBackground(m.capturedStart(j), m.capturedEnd(j), j);
                        }
                        int start = m.capturedStart();
                        int end = m.capturedEnd();
                        coloredLine.paintForeground(start, end, 1);
                    }
                    cols << toHtmlSpans(coloredLine, backgroundColors + grayZebraColors);
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

                ColoredLine coloredLine(line);

                if (before > 0 || after > 0) {
                    coloredLine.paintBackground(zebra[i] ? zebraColor1 : zebraColor2);
                }

                cols << toHtmlSpans(coloredLine, backgroundColors + grayZebraColors);

                //cols << Html::span(line, "black");
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
