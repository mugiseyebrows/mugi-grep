#include "searchresultrenderer.h"

#include "displayoptionswidget.h"
#include <QTextBrowser>
#include <QDir>
#include "searchtab.h"
#include <QDebug>
#include <QApplication>
#include "boolmap.h"
#include "htmldivs.h"
#include "colors.h"
#include "hunk.h"
#include <QDir>
#include "fileio.h"
#include "html.h"
#include "htmlstyle.h"

QString fileHref(const QString& path, int lineNumber) {
    return "file:///" + QDir::toNativeSeparators(path) +
            "?line=" + QString::number(lineNumber + 1);
}

QStringList fileNameLineNumber(const Colors& colors, bool showFileName, bool showLineNumber,
                                          const QString& relativePath, const QString& href,
                                          int lineNumber, const QString& separator) {
    QStringList cols;
    if (showFileName) {
        cols << Html::anchor(relativePath, href, colors.anchorColor()) << Html::span(separator, colors.separatorColor());
    }
    if (showLineNumber) {
        cols << Html::span(QString::number(lineNumber), colors.linenumberColor()) << Html::span(separator, colors.separatorColor());
    }
    return cols;
}

QStringList fileNameLineNumberContext(const QString& color,
                                      bool showFileName, bool showLineNumber,
                                          const QString& relativePath,
                                      const QString& href,
                                          int lineNumber) {
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

class LineHit {
public:
    LineHit(int lineNumber, const QStringList& spans, bool matched) : lineNumber(lineNumber), spans(spans), matched(matched) {

    }
    int lineNumber;
    QStringList spans;
    bool matched;
};

class DivHit {
public:

    DivHit() {

    }

    DivHit(const QString absolutePath, const  QString relativePath)
        : absolutePath(absolutePath), relativePath(relativePath) {

    }

    QList<LineHit> lines;
    QString absolutePath;
    QString relativePath;
    QString backgroundColor;

    bool isNull() const {
        return absolutePath.isEmpty();
    }

    void append(const LineHit& hit) {
        lines.append(hit);
    }

    QString render(const Colors& colors, bool showFileName, bool showLineNumber) const {
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
};

class DivContext {
public:

    DivContext() {

    }

    DivContext(const QString absolutePath, const  QString relativePath, int lineNumber, const QString& context)
        : absolutePath(absolutePath), relativePath(relativePath), lineNumber(lineNumber), context(context) {

    }

    bool isNull() const {
        return absolutePath.isEmpty();
    }

    QString absolutePath;
    QString relativePath;
    int lineNumber;
    QString backgroundColor;
    QString color;
    QString context;

    QString render(const Colors&, bool showFileName, bool showLineNumber) const {
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
};

class Divs {
public:
    void append(const DivContext& context) {
        divs.append(QPair<DivContext, DivHit>(context, DivHit()));
    }

    void append(const DivHit& hit) {
        divs.append(QPair<DivContext, DivHit>(DivContext(), hit));
    }

    QString render(const Colors& colors, bool showFileName, bool showLineNumber) {
        QStringList res;
        for(int i=0;i<divs.size();i++) {
            const auto& item = divs[i];
            const DivContext& context = item.first;
            const DivHit& hit = item.second;
            if (!context.isNull()) {
                res << context.render(colors, showFileName, showLineNumber);
            } else {
                res << hit.render(colors, showFileName, showLineNumber);
            }

        }
        return res.join("");
    }

    QList<QPair<DivContext, DivHit> > divs;
};


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

QStringList SearchResultRenderer::fileNameLineNumberContext(const Colors& colors, bool showFileName, bool showLineNumber,
                                          const QString& relativePath, const QString& href,
                                          int lineNumber, const QString& separator) {
    QStringList cols;
    if (showFileName) {
        //cols << Html::anchor(relativePath, href, "gray") << Html::span(separator, "gray");
        cols << Html::span(relativePath, "white") << Html::span(separator, "white");
    }
    if (showLineNumber) {
        cols << Html::span(QString::number(lineNumber), "white") << Html::span(separator, "white");
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
        bool value = i % 2 ? !(*initial) : (*initial);
        for(int j=range.first;j<=range.second;j++) {
            result[j] = value;
        }
    }

    if (ranges.size() % 2) {
        *initial = !(*initial);
    }
    return result;
}



void SearchResultRenderer::testDoZebra() {

    bool initial = true;

    auto m1 = BoolMap().true_(0,6).false_(8,12).map();
    auto m2 = doZebra(2,2,{2,4,10},&initial);

    Q_ASSERT(m1 == m2);

    m1 = BoolMap().true_(2,4).map();
    m2 = doZebra(0,0,{2,3,4},&initial);

    Q_ASSERT(m1 == m2);
    Q_ASSERT(initial == false);

    m1 = BoolMap().false_(1,3).true_(7,11).map();
    m2 = doZebra(1,1,{2,8,10},&initial);

    Q_ASSERT(m1 == m2);
    Q_ASSERT(initial == false);

    qDebug() << "testDoZebra passed";
}

QString rtrimed(const QString& line) {

    QString line_ = line;
    line_.remove(QRegularExpression("\\s+$"));
    return line_;
}



void SearchResultRenderer::appendSearch(const SearchHits& hits) {
    RegExp pattern = hits.pattern();

    DisplayOptions options = mTab->displayOptions();

    int linesBefore = options.linesBefore();
    int linesAfter = options.linesAfter();
    bool onlyMatched = !options.wholeLine();
    bool showFileName = options.fileName();
    bool showLineNumber = options.lineNumber();
    bool showContext = options.context();
    bool signature = options.signature();

    Colors colors(linesBefore > 0 || linesAfter > 0);

    //HtmlDivs divs;

    Divs divs;

    for (int j = 0; j < hits.size(); j++) {

        SearchHit hit = hits.hit(j);
        QSet<int> siblings = hit.siblings(linesBefore, linesAfter);
        QList<int> matched = hit.hits();

        //QMap<int, bool> zebra = doZebra(linesBefore, linesAfter, matched, &mZebra);

        int min_ = matched[0] - linesBefore;
        int max_ = matched.last() + linesAfter;

        QMap<int, QString> lines = hit.cache();

        QString mPath = hit.path();
        QString mRelativePath = hit.relativePath();

        QList<QList<int> > blocks;
        QList<int> block;

        for (int i = min_; i <= max_; i++) {
            if (matched.contains(i) || siblings.contains(i)) {
                block.append(i);
            } else {
                // flush
                if (block.size() > 0) {
                    blocks.append(block);
                    block = {};
                }
            }
        }
        if (block.size() > 0) {
            blocks.append(block);
            block = {};
        }



        for (int i=0;i<blocks.size();i++) {

            block = blocks[i];

            QString backgroundColor;
            QString contextColor = colors.contextColor();

            if (linesBefore > 0 || linesAfter > 0) {
                backgroundColor = mZebra ? colors.grayZebraColors[0] : colors.grayZebraColors[1];
                //contextColor = mZebra ? colors.grayZebraColors[1] : colors.grayZebraColors[0];
                mZebra = !mZebra;
            } else {
                //contextColor = colors.alternateBaseColor();
            }

            if (showContext) {
                int lineNumber = block[0];
                LineContextItem context = hit.context(lineNumber);
                if (!context.isNull()) {
                    lineNumber = context.begin;
                    QString text = signature ? context.name : context.shortName;
                    DivContext div(hit.path(), hit.relativePath(), lineNumber, text);
                    div.backgroundColor = backgroundColor;
                    div.color = contextColor;
                    divs.append(div);
                }
            }

            DivHit divHit(hit.path(), hit.relativePath());
            divHit.backgroundColor = backgroundColor;

            for (int k=0;k<block.size();k++) {
                int lineNumber = block[k];

                QString line = rtrimed(lines.value(lineNumber));

                if (matched.contains(lineNumber)) {

                    ColoredLine coloredLine(line);
                    QRegularExpression regexp = pattern.includeExp();
                    QRegularExpressionMatchIterator it = regexp.globalMatch(line);

                    while (it.hasNext()) {
                        QRegularExpressionMatch m = it.next();
                        int jmax = qMin(m.lastCapturedIndex(), colors.backgroundColors.size() - 1);
                        for (int j = 1; j <= jmax; j++) {
                            coloredLine.paintBackground(m.capturedStart(j), m.capturedEnd(j), j);
                        }
                        int start = m.capturedStart();
                        int end = m.capturedEnd();
                        coloredLine.paintForeground(start, end, 1);
                    }

                    QStringList spans = toHtmlSpans(coloredLine, colors.backgroundColors);
                    divHit.append(LineHit(lineNumber, spans, true));

                } else {

                    ColoredLine coloredLine(line);
                    QStringList spans = toHtmlSpans(coloredLine, colors.backgroundColors);
                    divHit.append(LineHit(lineNumber, spans, false));
                }
            }

            divs.append(divHit);
        }

    }

    mTab->textBrowser()->append(divs.render(colors, showFileName, showLineNumber));
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



void renderHunk(QStringList& res, const QString& path, Hunk& hunk, const QString& anchorColor) {
    QStringList value = hunk.value();
    QString text = QString("@@ %1,%2 %1,%2 @@").arg(hunk.line()).arg(hunk.count());
    QString anchor = Html::anchor(text, fileHref(path, hunk.line() - 1), anchorColor);
    res << HtmlDivs::div(anchor)
        << value;
}

void SearchResultRenderer::appendReplace(const SearchHits& hits) {

    DisplayOptions options = mTab->displayOptions();

    int linesBefore = options.linesBefore();
    int linesAfter = options.linesAfter();
    bool showFileName = options.fileName();
    bool showLineNumber = options.lineNumber();

    Colors colors(linesBefore > 0 || linesAfter > 0);

    HtmlDivs divs;

    for (int j = 0; j < hits.size(); j++) {

        SearchHit hit = hits.hit(j);
        QSet<int> siblings = hit.siblings(linesBefore, linesAfter);
        QList<int> matched = hit.hits();

        QMap<int, bool> zebra = doZebra(linesBefore, linesAfter, matched, &mZebra);

        int min_ = matched[0] - linesBefore;
        int max_ = matched[matched.size()-1] + linesAfter;

        QMap<int, QString> lines = hit.cache();
        QString mPath = hit.path();
        QString mRelativePath = hit.relativePath();

        for (int i = min_; i <= max_; i++) {
            if (matched.contains(i) || siblings.contains(i)) {
                QString line = lines[i];
                QString separator = matched.contains(i) ? ":" : "-";
                QStringList cols = fileNameLineNumber(colors, showFileName, showLineNumber,
                                                      mRelativePath, fileHref(mPath, i), i + 1, separator);
                cols << line;
                divs.append(cols.join(""), zebra[i] ? colors.grayZebraColors[0] : colors.grayZebraColors[1]);
            }
        }
    }

    divs.close();

    QString html = divs.divs().join("");
    if (html.isEmpty()) {
        return;
    }

    mTab->textBrowser()->append(html);
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

    Colors colors(linesBefore > 0 || linesAfter > 0);

    for(int j=0;j<hits.size();j++) {

        SearchHit hit = hits.hit(j);
        QSet<int> siblings = hit.siblings(linesBefore, linesAfter);
        QList<int> matched = hit.hits();
        QString mRelativePath = hit.relativePath();
        QString path = hit.path();

        QRegularExpression rx = exp.includeExp();

        //QString href = "file:///" + QDir::toNativeSeparators(path);

        res << HtmlDivs::div(Html::anchor(mRelativePath, fileHref(path, matched[0]), colors.anchorColor()));

        //QStringList oldLines;
        //QStringList newLines;

        Hunk hunk(colors.redColor, colors.greenColor);

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

                QString subj = Html::span("- ","blue",QString()) + Html::spanZebra(oldLine,QString(),QString(),colors.highlightedRedColor);
                QString repl = Html::span("+ ","blue",QString()) + Html::spanZebra(newLine,QString(),QString(),colors.highlightedGreenColor);

                hunk.replace(i + 1, subj, repl);

            } else if (siblings.contains(i)) {
                QString subj = Html::span("&nbsp;" + lines[i], QString());
                hunk.context(i + 1, subj);
            } else {
                if (!hunk.isEmpty()) {
                    renderHunk(res, path, hunk, colors.anchorColor());
                    hunk = Hunk(colors.redColor, colors.greenColor);
                }
            }
        }

        if (!hunk.isEmpty()) {
            renderHunk(res, path, hunk, colors.anchorColor());
            hunk = Hunk(colors.redColor, colors.greenColor);
        }
    }

    QString html = res.join("");
    if (html.isEmpty()) {
        return;
    }

    mTab->textBrowser()->append(html);
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

QString replace(const QString& line, QRegularExpression rx, const QVariantList& replacement_, bool preserveCase) {

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

    return newLine.join("");
}

ReplaceParams SearchResultRenderer::replaceParams(bool rename)
{
    SearchHits hits = mTab->hits();
    RegExpReplacement replacement__ = mTab->params().replacement();

    QString replacement = replacement__.pattern();
    bool preserveCase = replacement__.preserveCase();

    QVariantList replacement_ = tokenize(replacement);

    RegExp exp = hits.pattern();

    QRegularExpression rx = exp.includeExp();

    QList<ReplaceFile> files;

    for (int j=0;j<hits.size();j++) {

        SearchHit hit = hits.hit(j);

        ReplaceFile file(hit.path());

        QMap<int, QString> lines = hit.cache();

        QList<int> matched = hit.hits();

        foreach(int i, matched) {
            QString line = lines[i];
            QString newLine = replace(line, rx, replacement_, preserveCase);
            file.append(ReplaceItem(i, line, newLine));
        }

        files.append(file);
    }

    QList<QPair<QString,QString>> renames;

    if (rename) {
        QStringList nameHits = mTab->nameHits().hits();
        for(const QString& path: nameHits) {
            QString name = FileIO::nameFromPath(path);
            QString newName = replace(name, rx, replacement_, preserveCase);
            QString newPath = QDir(FileIO::dirName(path)).filePath(newName);
            renames.append(QPair<QString,QString>(path,newPath));
        }
    }

    return ReplaceParams(files, rename, renames);
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

