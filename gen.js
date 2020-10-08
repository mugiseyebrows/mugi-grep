const {CppClass, cpp, qt, mName, CppSignature, pointer, constRef, ref} = require('mugicpp')
const path = require('path')

let src = path.join(__dirname, 'src')

function SearchHit() {
    let c = new CppClass('SearchHit')
    let m = {
        path: qt.QString,
        relativePath: qt.QString,
        hits: 'QList<int>',
        cache: 'QMap<int, QString>'
    }
    c.constructor_()

    let s = new CppSignature()

    c.constructor_(s.signature(m, {cache: 'QMap<int, QString>()'}))
    for (let k in m) {
        c.member(mName(k), m[k])
    }

    c.method('siblings', 'QSet<int>', {before: cpp.int, after: cpp.int}, `
    QSet<int> result;
    foreach (int line, mHits) {
        for (int i = line - before; i <= line + after; i++) {
            if (i!=line) {
                result.insert(i);
            }
        }
    }
    return result;
    `).const_()

    c.method('read', cpp.void, {before: cpp.int, after: cpp.int}, `
    QSet<int> keys = mCache.keys().toSet();

    QSet<int> hits = mHits.toSet();
    QSet<int> siblings_ = siblings(before, after);

    if (keys.contains(hits) && keys.contains(siblings_)) {
        return;
    }
    QFile file(mPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "cannot open" << mPath;
        return;
    }
    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForName("UTF-8"));
    int i =0;
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        if (hits.contains(i) || siblings_.contains(i)) {
            mCache[i] = line;
        }
        i++;
    }
    `)
    c.include('QMetaType', true, true)
    c.metatype()
    c.write(src)
}

function SearchHits() {
    let c = new CppClass('SearchHits')
    let m = {
        search: 'RegExp',
        hits: 'QList<SearchHit>',
        total: cpp.int,
        complete: cpp.int,
    }
    c.constructor_()
    c.constructor_(m)
    for (let k in m) {
        c.member(mName(k), m[k])
    }
    c.method('append', cpp.void, 'const SearchHits& hits',`mHits.append(hits.hits());`)
    c.method('append', cpp.void, 'const SearchHit& hit',`mHits.append(hit);`)

    c.method('size', cpp.int, '', 'return mHits.size();').const_()

    c.method('hit', 'SearchHit&', 'int index', 'return mHits[index];');

    c.method('read',cpp.void, {before: cpp.int, after: cpp.int},
    `for(int i=0;i<mHits.size();i++) {
        mHits[i].read(before, after);
    }`)

    c.include('SearchHit')
    c.include('RegExp')
    c.include('QMetaType', true, true)
    c.metatype()
    c.write(src)
}

function SearchParams() {
    let c = new CppClass('SearchParams')
    let m = {
        id: cpp.int,
        path: qt.QString,
        search: 'RegExp',
        filter: 'RegExpPath',
        cacheFileList: cpp.bool,
        skipBinary: cpp.bool
    }
    c.constructor_({},{mId:-1})
    c.constructor_(m)
    for (let k in m) {
        c.member(mName(k), m[k])
    }

    c.include('QMetaType', true, true)
    c.include('RegExp')
    c.include('RegExpPath')
    c.metatype()
    c.write(src)
}

function SearchTab() {
    let c = new CppClass('SearchTab')
    c.inherits('QWidget')
    let m = {
        id: cpp.int,
        params: 'SearchParams',
        hits: 'SearchHits',
    }
    
    let s = new CppSignature()

    c.constructor_(s.signature({parent: pointer(qt.QWidget)}, {parent: 0}), {'QWidget': 'parent'},
    `
    QVBoxLayout* layout = new QVBoxLayout();
    mTextBrowser = new QTextBrowser();
    layout->addWidget(mTextBrowser);
    setLayout(layout);
    `)

    c.method('append',cpp.void,constRef('SearchHits','hits'),`mHits.append(hits); display();`)

    c.method('display', cpp.void, '', 
    `
    RegExp search = mHits.search();

    mTextBrowser->clear();

    int before = 0;
    int after = 0;

    mHits.read(before, after);

    QStringList res;

    for(int j=0;j<mHits.size();j++) {

        SearchHit& hit = mHits.hit(j);
        QSet<int> siblings = hit.siblings(before, after);
        QList<int> matched = hit.hits();

        static QStringList backgroundColors = {"#ffffff","#ffdfba","#baffc9","#bae1ff","#ffffba","#ffb3ba"};

        bool onlyMatched = false;
        bool showFileName = true;
        bool showLineNumber = true;

        int max1 = *std::max_element(matched.begin(), matched.end());
        int max2 = siblings.isEmpty() ? 0 : *std::max_element(siblings.begin(), siblings.end());
        int max_ = qMax(max1, max2);

        QMap<int, QString> lines = hit.cache();

        QString mPath = hit.path();
        QString mRelativePath = hit.relativePath();

        for(int i=0;i<=max_;i++) {

            if (matched.contains(i)) {

                QString line = lines[i];

                QRegularExpression regexp = search.includeExp();
                QRegularExpressionMatchIterator it = regexp.globalMatch(line);


                QString href = "file:///" + QDir::toNativeSeparators(mPath) + "?line=" + QString::number(i+1);

                if (onlyMatched) {
                    // each match on new line
                    while(it.hasNext()) {
                        QStringList cols = fileNameLineNumber(showFileName, showLineNumber, mRelativePath, href, i+1);
                        QRegularExpressionMatch m = it.next();
                        ColoredLine coloredLine(line);
                        int jmax = qMin(m.lastCapturedIndex(), backgroundColors.size() - 1);
                        for(int j=1;j<=jmax;j++) {
                            coloredLine.paintBackground(m.capturedStart(j), m.capturedEnd(j), j);
                        }
                        coloredLine.paintForeground(m.capturedStart(), m.capturedEnd(), 1);
                        cols << toHtmlSpans(coloredLine.mid(m.capturedStart(),m.capturedLength()), backgroundColors);
                        res << cols.join("");
                    }
                } else {
                    // all matches on one line
                    QStringList cols = fileNameLineNumber(showFileName, showLineNumber, mRelativePath, href, i+1);
                    ColoredLine coloredLine(line);
                    while(it.hasNext()) {
                        QRegularExpressionMatch m = it.next();
                        int jmax = qMin(m.lastCapturedIndex(), backgroundColors.size() - 1);
                        for(int j=1;j<=jmax;j++) {
                            coloredLine.paintBackground(m.capturedStart(j), m.capturedEnd(j), j);
                        }
                        coloredLine.paintForeground(m.capturedStart(), m.capturedEnd(), 1);
                    }
                    cols << toHtmlSpans(coloredLine, backgroundColors);
                    res << cols.join("");
                }

            } else if (siblings.contains(i)) {

                QString line = lines[i];
                QString href = "file:///" + QDir::toNativeSeparators(mPath) + "?line=" + QString::number(i+1);
                QStringList cols;
                if (showFileName) {
                    cols << Html::anchor(mRelativePath, href, "violet")
                         << Html::span("-", "blue");
                }
                if (showLineNumber) {
                    cols << Html::span(QString::number(i+1), "green")
                         << Html::span("-", "blue");
                }
                cols << Html::span(line, "black");
                res << cols.join("");
            }
        }

    }

    mTextBrowser->setText(res.join("<br/>"));
    `)

    c.method('toHtmlSpans', qt.QStringList, 'const ColoredLine& coloredLine, const QStringList& backgroundColors', 
    `QStringList cols;
    QList<ColoredLineSpan> spans = coloredLine.spans();
    foreach(const ColoredLineSpan& span, spans) {
        QString col = coloredLine.string().mid(span.start(), span.length());
        if (span.background() == 0 || span.background() >= backgroundColors.size()) {
            cols << Html::span(col, span.foreground() == 0 ? "black" : "red");
        } else {
            cols << Html::span(col, span.foreground() == 0 ? "black" : "red", backgroundColors[span.background()]);
        }
    }
    return cols;
    `).static()

    c.method('fileNameLineNumber', qt.QStringList, 'bool showFileName, bool showLineNumber, const QString& relativePath, const QString& href, int lineNumber',
    `QStringList cols;
    if (showFileName) {
        cols << Html::anchor(relativePath, href, "violet")
             << Html::span(":", "blue");
    }
    if (showLineNumber) {
        cols << Html::span(QString::number(lineNumber), "green")
        << Html::span(":", "blue");
    }
    return cols;
    `).static()



    //c.constructor_(m)
    for (let k in m) {
        c.member(mName(k), m[k], undefined, {getter: ['params','hits'].indexOf(k) < 0})
    }

    c.member('mTextBrowser', 'QTextBrowser*')

    c.method('params', 'SearchParams&', '', `return mParams;`)
    c.method('hits', 'SearchHits&','','return mHits;')

    c.include('SearchHits')
    c.include('SearchParams')
    c.include('Utils')
    c.include('Html')
    c.include('ColoredLine')

    c.qobject()
    c.write(src)
}

SearchHit()
SearchHits()
SearchParams()
SearchTab()