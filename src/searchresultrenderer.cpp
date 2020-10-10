#include "searchresultrenderer.h"

#include "displayoptionswidget.h"
#include <QTextBrowser>
#include <QDir>
#include "searchtab.h"
#include <QDebug>

SearchResultRenderer::SearchResultRenderer(QObject *parent) : QObject(parent), mZebra(false), mMode(Search)
{

}

void SearchResultRenderer::setTab(SearchTab *tab)
{
    mTab = tab;
    connect(tab->displayOptionsWidget(),SIGNAL(optionsChanged()),this,SLOT(onOptionsChanged()));
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

void SearchResultRenderer::appendSearch(const SearchHits& hits) {
    RegExp pattern = hits.pattern();

    DisplayOptions options = mTab->displayOptions();

    int linesBefore = options.linesBefore();
    int linesAfter = options.linesAfter();

    //hits.read(before, after);

    QStringList res;

    for (int j = 0; j < hits.size(); j++) {

        SearchHit hit = hits.hit(j);
        QSet<int> siblings = hit.siblings(linesBefore, linesAfter);
        QList<int> matched = hit.hits();

        QMap<int, bool> zebra = doZebra(linesBefore, linesAfter, matched);

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

                    if (linesBefore > 0 || linesAfter > 0) {
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

                if (linesBefore > 0 || linesAfter > 0) {
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


int backreferenceLength(const QString& replacement, int pos) {
    if (replacement[pos] == '\\' && pos + 1 < replacement.size() && replacement[pos+1].isDigit()) {
        int len = 1;
        while(++pos < replacement.size()) {
            if (!replacement[pos].isDigit()) {
                break;
            }
            len++;
        }
        return len;
    }
    return -1;
}


QVariantList tokenize(const QString& replacement) {
    QVariantList result;
    int prev = 0;
    int i = 0;
    while (i < replacement.size()) {
        int len = backreferenceLength(replacement, i);
        if (len > 0) {
            if (i - prev > 0) {
                QString part = replacement.mid(prev,i - prev);
                result.append(part);
            }
            QString ref = replacement.mid(i+1,len-1);
            result.append(ref.toInt());
            prev = i + len;
            i += len;
        } else {
            i++;
        }
    }
    if (prev < replacement.size()) {
        QString part = replacement.mid(prev);
        result.append(part);
    }
    return result;
}


QString sameCase(const QString& repl, const QString& orig) {
    if (orig.toLower() == orig) {
        return repl.toLower();
    } else if (orig.toUpper() == orig) {
        return repl.toUpper();
    }
    return repl;
}


QString withBackreferences(const QRegularExpressionMatch& m, const QVariantList& replacement, bool preserveCase) {
    QStringList result;
    foreach(const QVariant& v, replacement) {
        if (v.type() == QVariant::Int) {
            result.append(m.captured(v.toInt()));
        } else if (v.type() == QVariant::String) {
            result.append(v.toString());
        } else {
            qDebug() << "withBackreferences error:" << v;
        }
    }
    QString result_ = result.join("");
    return preserveCase ? sameCase(result_, m.captured(0)) : result_;
}


#include "hunk.h"

void SearchResultRenderer::appendPreview(const SearchHits& hits) {

    QStringList res;

    RegExp exp = hits.pattern();
    QString replacement = mReplacement.pattern();
    bool preserveCase = mReplacement.preserveCase();

    DisplayOptions options = mTab->displayOptions();

    int linesBefore = options.linesBefore();
    int linesAfter = options.linesAfter();

    for(int j=0;j<hits.size();j++) {

        SearchHit hit = hits.hit(j);
        QSet<int> siblings = hit.siblings(linesBefore, linesAfter);
        QList<int> matched = hit.hits();
        QString mRelativePath = hit.relativePath();
        QString path = hit.path();

        QRegularExpression rx = exp.includeExp();
        QVariantList replacement_ = tokenize(replacement);
        QString href = "file:///" + QDir::toNativeSeparators(path);
        res << Html::anchor(mRelativePath, href, "violet");

        QStringList oldLines;
        QStringList newLines;

        Hunk hunk;

        QString lightRed = "#FFD9DD";
        QString red = "#fdb8c0";
        QString lightGreen = "#D9FFE3";
        QString green = "#acf2bd";

        QMap<int, QString> lines = hit.cache();

        int max1 = *std::max_element(matched.begin(), matched.end());
        int max2 = siblings.isEmpty() ? 0 : *std::max_element(siblings.begin(), siblings.end());
        int max_ = qMax(max1, max2);

        for(int i=0;i<max_;i++) {
            if (matched.contains(i)) {
                QString line = lines[i];
                QRegularExpressionMatchIterator it = rx.globalMatch(line);
                QStringList oldLine;
                QStringList newLine;
                int prev = 0;
                while(it.hasNext()) {
                    QRegularExpressionMatch m = it.next();
                    QString s = line.mid(prev,m.capturedStart() - prev);
                    oldLine << s;
                    newLine << s;

                    s = line.mid(m.capturedStart(),m.capturedLength());
                    oldLine << s;
                    newLine << withBackreferences(m,replacement_,preserveCase);
                    prev = m.capturedEnd();
                }

                if (prev < line.size()) {
                    oldLine << line.mid(prev);
                    newLine << line.mid(prev);
                }

                Q_ASSERT(oldLine.join("") == line);

                QString subj = Html::span("- ","blue",lightRed) + Html::spanZebra(oldLine,"black",lightRed,red);
                QString repl = Html::span("+ ","blue",lightGreen) + Html::spanZebra(newLine,"black",lightGreen,green);

                hunk.replace(i + 1, subj, repl);
                oldLines << oldLine.join("");
                newLines << newLine.join("");
            } else if (siblings.contains(i)) {
                QString subj = Html::span("&nbsp;&nbsp;" + lines[i], "black");
                hunk.context(i + 1, subj);
            } else {
                if (!hunk.isEmpty()) {
                    QStringList value = hunk.value();
                    res << Html::span(QString("@@ %1,%2 %1,%2 @@").arg(hunk.line()).arg(hunk.count()), "blue")
                        << value;
                    hunk = Hunk();
                }
            }
        }

        if (!hunk.isEmpty()) {
            QStringList value = hunk.value();
            res << Html::span(QString("@@ %1,%2 %1,%2 @@").arg(hunk.line()).arg(hunk.count()), "blue")
                << value;
            hunk = Hunk();
        }

        QList<ReplacementLine> replacementLines;
        for(int i=0;i<oldLines.size();i++) {
            replacementLines.append(ReplacementLine(matched[i],oldLines[i],newLines[i]));
        }

        replacements.append(Replacement(path,replacementLines));



    }


    mTab->textBrowser()->append(res.join("<br/>"));

}

void SearchResultRenderer::append(const SearchHits& hits) {

    if (mMode == Search) {
        appendSearch(hits);
    } else {
        appendPreview(hits);
    }

}

void SearchResultRenderer::setMode(SearchResultRenderer::Mode mode)
{
    mMode = mode;
    mTab->trigRerender();
}

void SearchResultRenderer::setReplacement(const RegExpReplacement &value)
{
    mReplacement = value;
}

void SearchResultRenderer::onOptionsChanged() {
    DisplayOptions options = mTab->displayOptions();
    mTab->textBrowser()->clear();
    mTab->hits().read(options.linesBefore(), options.linesAfter());
    append(mTab->hits());
}
