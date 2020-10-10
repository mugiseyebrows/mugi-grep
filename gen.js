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
    qDebug() << "read" << mPath;
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
        pattern: 'RegExp',
        hits: 'QList<SearchHit>',
        total: cpp.int,
        complete: cpp.int,
    }
    c.constructor_()
    c.constructor_(m)

    c.constructor_('const RegExp& pattern, QList<SearchHit> hits')

    for (let k in m) {
        c.member(mName(k), m[k], {total: -1, complete: -1}[k])
    }
    c.method('append', cpp.void, 'const SearchHits& hits',`mHits.append(hits.hits());`)
    c.method('append', cpp.void, 'const SearchHit& hit',`mHits.append(hit);`)

    c.method('size', cpp.int, '', 'return mHits.size();').const_()

    c.method('isEmpty', cpp.bool, '', 'return mHits.isEmpty();').const_()

    c.method('hit', 'SearchHit', 'int index', 'return mHits[index];').const_()

    c.method('mid','SearchHits', 'int index', 'return SearchHits(mPattern, mHits.mid(index));')

    c.method('read',cpp.void, {before: cpp.int, after: cpp.int},
    `for(int i=0;i<mHits.size();i++) {
        mHits[i].read(before, after);
    }`)

    c.method('clear',cpp.void,'','mHits.clear();')

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
        pattern: 'RegExp',
        filter: 'RegExpPath',
        cacheFileList: cpp.bool,
        skipBinary: cpp.bool
    }
    c.constructor_()
    //c.constructor_(m)
    for (let k in m) {
        c.member(mName(k), m[k], {id: -1}[k])
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
        params: 'SearchParams',
        hits: 'SearchHits',
    }
    
    let s = new CppSignature()

    c.constructor_(s.signature({parent: pointer(qt.QWidget)}, {parent: 0}), {'QWidget': 'parent'},
    `
    QVBoxLayout* layout = new QVBoxLayout();
    mTextBrowser = new QTextBrowser();
    #ifdef Q_OS_LINUX
        QFont font("Liberation Mono", 11, QFont::Normal);
        mTextBrowser->setFont(font);
    #endif
    #ifdef Q_OS_WIN
        QFont font("Courier New", 12, QFont::Normal);
        mTextBrowser->setFont(font);
    #endif
    mTextBrowser->setOpenLinks(false);
    mDisplayOptions = new DisplayOptionsWidget();
    mRenderer = new SearchResultRenderer();
    layout->addWidget(mTextBrowser);
    layout->addWidget(mDisplayOptions);
    setLayout(layout);
    mRenderer->setTab(this);
    `)

    c.method('append',cpp.void, constRef('SearchHits','hits'),
    `mHits.setPattern(hits.pattern());
    if (hits.isEmpty()) {
        return;
    }
    int size = mHits.size();
    mHits.append(hits);
    read();
    mRenderer->append(mHits.mid(size));
    `)

    c.method('read', cpp.void, '', 
    `int linesBefore = mDisplayOptions->linesBefore();
    int linesAfter = mDisplayOptions->linesAfter();
    mHits.read(linesBefore, linesAfter);`)

    c.method('trigRerender', cpp.void, '', 'mDisplayOptions->trigChanged();')

    //c.constructor_(m)
    for (let k in m) {
        c.member(mName(k), m[k], {id: -1}[k], {getter: ['params','hits'].indexOf(k) < 0})
    }

    c.member('mTextBrowser', 'QTextBrowser*')
    c.member('mDisplayOptions','DisplayOptionsWidget*')
    c.member('mRenderer','SearchResultRenderer*')

    c.method('params', 'SearchParams&', '', `return mParams;`)
    c.method('hits', 'SearchHits&','','return mHits;')

    c.include('SearchHits')
    c.include('SearchParams')
    c.include('Utils')
    c.include('Html')
    c.include('ColoredLine')
    c.include('DisplayOptionsWidget')
    c.include('SearchResultRenderer')

    c.qobject()
    c.write(src)
}

SearchHit()
SearchHits()
SearchParams()
SearchTab()