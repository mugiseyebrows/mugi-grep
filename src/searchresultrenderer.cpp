#include "searchresultrenderer.h"

#include "displayoptionswidget.h"
#include <QTextBrowser>
#include <QDir>
#include "searchtab.h"
#include <QDebug>
#include <QApplication>


bool isDarker(const QColor& color1, const QColor& color2) {
    double v1 = color1.valueF();
    double v2 = color2.valueF();
    return v1 < v2;
}


SearchResultRenderer::SearchResultRenderer(QObject *parent) : QObject(parent), mTab(0), mZebra(false)
{

}

void SearchResultRenderer::setTab(SearchTab *tab)
{
    mTab = tab;
    connect(tab->displayOptionsWidget(),SIGNAL(optionsChanged()),this,SLOT(onOptionsChanged()));
}

QString sameCase(const QString& repl, const QString& orig) {
    if (orig.toLower() == orig) {
        return repl.toLower();
    } else if (orig.toUpper() == orig) {
        return repl.toUpper();
    }
    return repl;
}

QString fileHref(const QString& path, int lineNumber) {
    return "file:///" + QDir::toNativeSeparators(path) +
            "?line=" + QString::number(lineNumber + 1);
}

QStringList SearchResultRenderer::toHtmlSpans(const ColoredLine& coloredLine,
                                   const QStringList& backgroundColors) {
    QStringList cols;

    //QString foreground = mDarkMode ? "white" : "black";

    QList<ColoredLineSpan> spans = coloredLine.spans();
    foreach (const ColoredLineSpan& span, spans) {
        QString col = coloredLine.string().mid(span.start(), span.length());
        if (span.background() == 0 || span.background() >= backgroundColors.size()) {
            cols << Html::span(col, span.foreground() == 0 ? QString() : "red");
        } else {
            cols << Html::span(col, span.foreground() == 0 ? QString() : "red",
                               backgroundColors[span.background()]);
        }
    }
    return cols;
}

enum class TextCase {
    Lower,
    Upper,
    Capitalized,
    Mixed
};

TextCase textCase(const QString& text) {

    if (text.isEmpty()) {
        return TextCase::Lower;
    }
    bool firstUpper = text[0].isLetter() && text[0].isUpper();
    bool restLower = true;
    for(int i=1;i<text.size();i++) {
        if (text[i].isLetter() && text[i].isUpper()) {
            restLower = false;
            break;
        }
    }
    if (firstUpper && restLower) {
        return TextCase::Capitalized;
    }
    bool lower = false;
    bool upper = false;
    for(int i=0;i<text.size();i++) {
        if (text[i].isLetter()) {
            if (text[i].isLower()) {
                lower = true;
            } else if (text[i].isUpper()){
                upper = true;
            }
        }
    }
    if (lower && upper) {
        return TextCase::Mixed;
    }
    if (lower && !upper) {
        return TextCase::Lower;
    }
    if (!lower && upper) {
        return TextCase::Upper;
    }

    return TextCase::Lower;
}

QString toTextCase(const QString& text, TextCase textCase) {
    if (textCase == TextCase::Capitalized) {
        if (text.isEmpty()) {
            return QString();
        }
        return text.mid(0,1).toUpper() + text.mid(1).toLower();
    } else if (textCase == TextCase::Upper) {
        return text.toUpper();
    } else if (textCase == TextCase::Lower) {
        return text.toLower();
    }
    return text;
}


QString withBackreferences(const QRegularExpressionMatch& m, const QVariantList& replacement, bool preserveCase) {
    QStringList result;

    TextCase case1 = textCase(m.captured(0));

    foreach(const QVariant& v, replacement) {
        if (v.type() == QVariant::Int) {
            QString captured = m.captured(v.toInt());
            if (preserveCase) {
                //if (case1 == TextCase::Lower || case1 == TextCase::Upper || case1 == TextCase::Capitalized) {
                    captured = toTextCase(captured, case1);
                //}
            }
            result.append(captured);
        } else if (v.type() == QVariant::String) {
            QString text = v.toString();
            if (preserveCase) {
                //if (case1 == TextCase::Lower || case1 == TextCase::Upper || case1 == TextCase::Capitalized) {
                    text = toTextCase(text, case1);
                //}
            }
            result.append(text);
        } else {
            qDebug() << "withBackreferences error:" << v;
        }
    }
    return result.join("");
}

QStringList SearchResultRenderer::fileNameLineNumber(bool showFileName, bool showLineNumber,
                                          const QString& relativePath, const QString& href,
                                          int lineNumber,
                                            const QString& separator,
                                         const QString& anchorColor,
                                         const QString& separatorColor,
                                         const QString& linenumberColor) {
    QStringList cols;
    if (showFileName) {
        cols << Html::anchor(relativePath, href, anchorColor) << Html::span(separator, separatorColor);
    }
    if (showLineNumber) {
        cols << Html::span(QString::number(lineNumber), linenumberColor) << Html::span(separator, separatorColor);
    }
    return cols;
}



QMap<int, bool> SearchResultRenderer::doZebra(int before, int after, const QList<int>& matched, bool* initial) {


    QList<QPair<int,int> > ranges;
    for(int match: matched) {
        ranges.append(QPair<int,int>(match - before, match + after));
    }

    int i = 0;
    while (i < ranges.size()-1) {
        auto range1 = ranges[i];
        auto range2 = ranges[i+1];
        if (range2.first - 1 <= range1.second) {
            ranges.removeAt(i+1);
            ranges[i] = QPair<int,int>(range1.first, range2.second);
            i--;
        }
        i++;
    }

    QMap<int, bool> result;

    for(int i=0;i<ranges.size();i++) {
        auto range = ranges[i];
        for(int j=range.first;j<=range.second;j++) {
            result[j] = i % 2 ? !(*initial) : (*initial);
        }
    }

    /*for(int j=0;j<matched.size();j++) {
        int match = matched[j];
        for(int i=match-before;i<match+after+1;i++) {
            result[i] = j % 2 ? mZebra : !mZebra;
        }
    }*/
    if (ranges.size() % 2) {
        *initial = !(*initial);
    }
    return result;
}


class BoolRanges {
public:
    BoolRanges() {

    }

    BoolRanges& fill(int begin, int end, bool value) {
        for(int i=begin;i<=end;i++) {
            mMap[i] = value;
        }
        return *this;
    }

    BoolRanges& true_(int begin, int end) {
        return fill(begin, end, true);
    }
    BoolRanges& false_(int begin, int end) {
        return fill(begin, end, false);
    }
    QMap<int,bool> map() {
        return mMap;
    }

    QMap<int,bool> mMap;
};

void SearchResultRenderer::testDoZebra() {

    bool initial = true;

    auto m1 = BoolRanges().true_(0,6).false_(8,12).map();
    auto m2 = doZebra(2,2,{2,4,10},&initial);

    Q_ASSERT(m1 == m2);

    m1 = BoolRanges().true_(2,4).map();
    m2 = doZebra(0,0,{2,3,4},&initial);

    Q_ASSERT(m1 == m2);
    Q_ASSERT(initial == false);

    m1 = BoolRanges().false_(1,3).true_(7,11).map();
    m2 = doZebra(1,1,{2,8,10},&initial);

    Q_ASSERT(m1 == m2);
    Q_ASSERT(initial == false);

    qDebug() << "testDoZebra passed";
}


const QString trimRight(const QString& line) {
    int p = line.lastIndexOf(QRegularExpression("\\r?\\n"));
    if (p > -1) {
        return line.mid(0,p);
    }
    return line;
}

#include <QApplication>
#include "htmldivs.h"

void SearchResultRenderer::appendSearch(const SearchHits& hits) {
    RegExp pattern = hits.pattern();

    DisplayOptions options = mTab->displayOptions();

    int linesBefore = options.linesBefore();
    int linesAfter = options.linesAfter();
    bool onlyMatched = !options.wholeLine();
    bool showFileName = options.fileName();
    bool showLineNumber = options.lineNumber();

    //hits.read(before, after);

    //QStringList res;

    QPalette palette = qApp->palette();

    bool darkStyle = isDarker(palette.color(QPalette::Base), QColor(128,128,128));

    QString baseColor = palette.color(QPalette::Base).name();
    QString alternateBaseColor = palette.color(QPalette::AlternateBase).name();
    QString anchorColor = palette.color(QPalette::Link).name();
    QString separatorColor = darkStyle ? "#74c69d" : "#2d6a4f" ;
    QString linenumberColor = darkStyle ? "#e85d04" : "#dc2f02";

    HtmlDivs divs;

    for (int j = 0; j < hits.size(); j++) {

        SearchHit hit = hits.hit(j);
        QSet<int> siblings = hit.siblings(linesBefore, linesAfter);
        QList<int> matched = hit.hits();

        QMap<int, bool> zebra = doZebra(linesBefore, linesAfter, matched, &mZebra);

        QStringList backgroundColors;
        QStringList grayZebraColors;

        if (darkStyle) {
            backgroundColors = QStringList {QString(), "#09194f", "#70041b", "#560100", "#151350", "#3f1440", "#113302"};
        } else {
            backgroundColors = QStringList {QString(), "#ffdfba", "#baffc9", "#bae1ff", "#ffffba", "#ffb3ba"};
        }

        /*if (mDarkMode) {
            grayZebraColors = QStringList {"#191919", "#494949"};
        } else {
            grayZebraColors = QStringList {"#E6E6E6", "#F0F0F0"};
        }*/

        grayZebraColors = QStringList {baseColor, alternateBaseColor};

        if (linesBefore == 0 && linesAfter == 0) {
            grayZebraColors = QStringList {QString(), QString()};
        }

        /*int zebraColor1 = backgroundColors.size();
        int zebraColor2 = zebraColor1 + 1;*/

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

                if (onlyMatched) {
                    // each match on new line
                    while (it.hasNext()) {
                        QStringList cols = fileNameLineNumber(showFileName, showLineNumber,
                                                              mRelativePath,
                                                              fileHref(mPath, i), i + 1,
                                                              ":",anchorColor, separatorColor, linenumberColor);
                        QRegularExpressionMatch m = it.next();
                        ColoredLine coloredLine(line);
                        int jmax = qMin(m.lastCapturedIndex(), backgroundColors.size() - 1);
                        for (int j = 1; j <= jmax; j++) {
                            coloredLine.paintBackground(m.capturedStart(j), m.capturedEnd(j), j);
                        }
                        coloredLine.paintForeground(m.capturedStart(), m.capturedEnd(), 1);
                        cols << toHtmlSpans(coloredLine.mid(m.capturedStart(), m.capturedLength()),
                                            backgroundColors);

                        //res << cols.join("");

                        divs.append(cols.join(""), zebra[i] ? grayZebraColors[0] : grayZebraColors[1]);

                    }
                } else {
                    // all matches on one line
                    QStringList cols = fileNameLineNumber(showFileName, showLineNumber,
                                                          mRelativePath, fileHref(mPath, i), i + 1,
                                                          ":", anchorColor, separatorColor, linenumberColor);
                    ColoredLine coloredLine(line);

                    /*if (linesBefore > 0 || linesAfter > 0) {
                        coloredLine.paintBackground(zebra[i] ? zebraColor1 : zebraColor2);
                    }*/

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
                    cols << toHtmlSpans(coloredLine, backgroundColors);
                    //res << cols.join("");

                    divs.append(cols.join(""), zebra[i] ? grayZebraColors[0] : grayZebraColors[1]);
                }

            } else if (siblings.contains(i)) {

                QString line = lines[i];


                QString href = fileHref(mPath, i + 1);

                QStringList cols = fileNameLineNumber(showFileName, showLineNumber, mRelativePath, href, i + 1, "-", anchorColor, separatorColor, linenumberColor);
                /*if (showFileName) {
                    cols << Html::anchor(mRelativePath, href, "violet") << Html::span("-", "blue");
                }
                if (showLineNumber) {
                    cols << Html::span(QString::number(i + 1), "green") << Html::span("-", "blue");
                }*/

                ColoredLine coloredLine(line);

                /*if (linesBefore > 0 || linesAfter > 0) {
                    coloredLine.paintBackground(zebra[i] ? zebraColor1 : zebraColor2);
                }*/

                cols << toHtmlSpans(coloredLine, backgroundColors);

                //cols << Html::span(line, "black");

                /*if (cols.size() == 1) {
                    qDebug() << 1;
                }*/

                //res << cols.join("");

                divs.append(cols.join(""), zebra[i] ? grayZebraColors[0] : grayZebraColors[1]);

            }
        }
    }
    //QString html = res.join("<br/>");

    divs.close();

    QString html = divs.divs().join("");

    if (html.isEmpty()) {
        return;
    }

    /*static int i = 0;
    dump(QString("D:\\w\\%1.html").arg(i), html);*/

    mTab->textBrowser()->append(html);
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





#include "hunk.h"

void renderHunk(QStringList& res, const QString& path, Hunk& hunk) {
    QStringList value = hunk.value();
    QString text = QString("@@ %1,%2 %1,%2 @@").arg(hunk.line()).arg(hunk.count());
    res << Html::anchor(text, fileHref(path, hunk.line() - 1), "blue")
        << value;
}

void SearchResultRenderer::appendReplace(const SearchHits& hits) {


    DisplayOptions options = mTab->displayOptions();

    int linesBefore = options.linesBefore();
    int linesAfter = options.linesAfter();
    //bool onlyMatched = !options.wholeLine();
    bool showFileName = options.fileName();
    bool showLineNumber = options.lineNumber();

    QPalette palette = qApp->palette();
    QString base = palette.color(QPalette::Base).name();
    QString alternateBase = palette.color(QPalette::AlternateBase).name();
    QString anchorColor = palette.color(QPalette::Link).name();
    QString separatorColor = "violet";
    QString linenumberColor = "green";

    //hits.read(before, after);

    QStringList res;

    for (int j = 0; j < hits.size(); j++) {

        SearchHit hit = hits.hit(j);
        QSet<int> siblings = hit.siblings(linesBefore, linesAfter);
        QList<int> matched = hit.hits();

        QMap<int, bool> zebra = doZebra(linesBefore, linesAfter, matched, &mZebra);

        static QStringList backgroundColors = {"#ffffff", "#ffdfba", "#baffc9",
                                               "#bae1ff", "#ffffba", "#ffb3ba"};

        static QStringList grayZebraColors = {"#E6E6E6", "#F0F0F0"};

        int zebraColor1 = backgroundColors.size();
        int zebraColor2 = zebraColor1 + 1;

        int max1 = *std::max_element(matched.begin(), matched.end());
        int max2 = siblings.isEmpty() ? 0 : *std::max_element(siblings.begin(), siblings.end());
        int max_ = qMax(max1, max2);

        QMap<int, QString> lines = hit.cache();

        QString mPath = hit.path();
        QString mRelativePath = hit.relativePath();

        for (int i = 0; i <= max_; i++) {

            if (matched.contains(i)) {

                QString line = lines[i];

                //qDebug() << pattern.includeExp().pattern();

                QStringList cols = fileNameLineNumber(showFileName, showLineNumber,
                                                      mRelativePath, fileHref(mPath, i), i + 1,
                                                      ":", anchorColor, separatorColor, linenumberColor);
                ColoredLine coloredLine(line);

                if (linesBefore > 0 || linesAfter > 0) {
                    coloredLine.paintBackground(zebra[i] ? zebraColor1 : zebraColor2);
                }

                cols << toHtmlSpans(coloredLine, backgroundColors + grayZebraColors);
                res << cols.join("");

            } else if (siblings.contains(i)) {

                QString line = lines[i];
                QStringList cols;
                if (showFileName) {
                    cols << Html::anchor(mRelativePath, fileHref(mPath, i), "violet") << Html::span("-", "blue");
                }
                if (showLineNumber) {
                    cols << Html::span(QString::number(i + 1), "green") << Html::span("-", "blue");
                }

                ColoredLine coloredLine(line);

                if (linesBefore > 0 || linesAfter > 0) {
                    coloredLine.paintBackground(zebra[i] ? zebraColor1 : zebraColor2);
                }

                cols << toHtmlSpans(coloredLine, backgroundColors + grayZebraColors);

                res << cols.join("");
            }
        }
    }
    mTab->textBrowser()->append(res.join("<br/>"));
}

void SearchResultRenderer::appendPreview(const SearchHits& hits) {

    QStringList res;

    RegExp exp = hits.pattern();

    RegExpReplacement replacement__ = mTab->params().replacement();

    QString replacement = replacement__.pattern();
    bool preserveCase = replacement__.preserveCase();

    DisplayOptions options = mTab->displayOptions();

    int linesBefore = options.linesBefore();
    int linesAfter = options.linesAfter();

    QVariantList replacement_ = tokenize(replacement);

    for(int j=0;j<hits.size();j++) {

        SearchHit hit = hits.hit(j);
        QSet<int> siblings = hit.siblings(linesBefore, linesAfter);
        QList<int> matched = hit.hits();
        QString mRelativePath = hit.relativePath();
        QString path = hit.path();

        QRegularExpression rx = exp.includeExp();

        //QString href = "file:///" + QDir::toNativeSeparators(path);
        res << Html::anchor(mRelativePath, fileHref(path, matched[0]), "violet");

        //QStringList oldLines;
        //QStringList newLines;

        Hunk hunk;

        QString lightRed = "#FFD9DD";
        QString red = "#fdb8c0";
        QString lightGreen = "#D9FFE3";
        QString green = "#acf2bd";

        QMap<int, QString> lines = hit.cache();

        int max1 = *std::max_element(matched.begin(), matched.end());
        int max2 = siblings.isEmpty() ? 0 : *std::max_element(siblings.begin(), siblings.end());
        int max_ = qMax(max1, max2);

        for(int i=0;i<=max_;i++) {
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
                //oldLines << oldLine.join("");
                //newLines << newLine.join("");
            } else if (siblings.contains(i)) {
                QString subj = Html::span("&nbsp;&nbsp;" + lines[i], "black");
                hunk.context(i + 1, subj);
            } else {
                if (!hunk.isEmpty()) {
                    /*QStringList value = hunk.value();
                    res << Html::span(QString("@@ %1,%2 %1,%2 @@").arg(hunk.line()).arg(hunk.count()), "blue")
                        << value;*/
                    renderHunk(res, path, hunk);
                    hunk = Hunk();
                }
            }
        }

        if (!hunk.isEmpty()) {
            /*QStringList value = hunk.value();
            res << Html::span(QString("@@ %1,%2 %1,%2 @@").arg(hunk.line()).arg(hunk.count()), "blue")
                << value;*/
            renderHunk(res, path, hunk);
            hunk = Hunk();
        }
    }


    mTab->textBrowser()->append(res.join("<br/>"));

}



void SearchResultRenderer::append(const SearchHits& hits) {
    if (mTab->mode() == Mode::Search) {
        appendSearch(hits);
    } else if (mTab->mode() == Mode::Preview){
        appendPreview(hits);
    } else if (mTab->mode() == Mode::Replace) {
        appendReplace(hits);
    }
}

ReplaceParams SearchResultRenderer::replaceParams()
{
    SearchHits hits = mTab->hits();
    RegExpReplacement replacement__ = mTab->params().replacement();

    QString replacement = replacement__.pattern();
    bool preserveCase = replacement__.preserveCase();

    QVariantList replacement_ = tokenize(replacement);

    RegExp exp = hits.pattern();

    QRegularExpression rx = exp.includeExp();

    ReplaceParams result;

    for (int j=0;j<hits.size();j++) {

        SearchHit hit = hits.hit(j);

        ReplaceFile file(hit.path());

        QMap<int, QString> lines = hit.cache();

        QList<int> matched = hit.hits();

        foreach(int i, matched) {

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

            file.append(ReplaceItem(i, oldLine.join(""), newLine.join("")));
        }

        result.append(file);
    }

    return result;
}

void SearchResultRenderer::onOptionsChanged() {
    if (!mTab) {
        return;
    }

    DisplayOptions options = mTab->displayOptions();
    mTab->textBrowser()->clear();
    mTab->hits().read(options.linesBefore(), options.linesAfter());
    append(mTab->hits());
}


void compare(const QVariantList& e, const QVariantList& a) {
    if (e == a) {
        return;
    }
    QString e_ = "{" + Utils::toStringList(e).join("|") + "}";
    QString a_ = "{" + Utils::toStringList(a).join("|") + "}";
    qDebug() << "not equal, expected: " << e_.toStdString().c_str() << ", actual " << a_.toStdString().c_str();
}


void SearchResultRenderer::testTokenize() {

    qDebug() << "testTokenize() started";

    QString t;
    QVariantList a,e;

    t = "foo\\1bar\\2baz";
    e = {"foo",1,"bar",2,"baz"};
    a = tokenize(t);
    compare(e,a);

    t = "foo\\1bar\\2baz\\3";
    e = {"foo",1,"bar",2,"baz",3};
    a = tokenize(t);
    compare(e,a);

    t = "foo\\1bar\\2baz\\3a";
    e = {"foo",1,"bar",2,"baz",3,"a"};
    a = tokenize(t);
    compare(e,a);

    t = "\\1bar\\2baz\\3";
    e = {1,"bar",2,"baz",3};
    a = tokenize(t);
    compare(e,a);

    t = "\\1bar\\2\\3baz\\52";
    e = {1,"bar",2,3,"baz",52};
    a = tokenize(t);
    compare(e,a);

    qDebug() << "testTokenize() finished";

}

