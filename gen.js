const {CppClass, cpp, qt, mName, CppSignature, pointer, constRef, ref, sc} = require('mugicpp')
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

    c.method('clearCache', cpp.void, '', 'mCache = QMap<int, QString>();')

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
        filtered: cpp.int,
        last: qt.QString
    }
    c.constructor_()
    //c.constructor_(m)
    //c.constructor_('int mode, const RegExp& pattern, QList<SearchHit> hits')
    c.constructor_('const RegExp& pattern, QList<SearchHit> hits = QList<SearchHit>()')
    //c.constructor_('const RegExp& pattern')

    for (let k in m) {
        c.member(mName(k), m[k], {total: -1, complete: -1, filtered: -1}[k])
    }
    c.method('append', cpp.void, 'const SearchHits& hits',`mPattern = hits.pattern(); mHits.append(hits.hits());`)
    c.method('append', cpp.void, 'const SearchHit& hit',`mHits.append(hit);`)

    c.method('size', cpp.int, '', 'return mHits.size();').const_()

    c.method('isEmpty', cpp.bool, '', 'return mHits.isEmpty();').const_()

    c.method('hit', 'SearchHit', 'int index', 'return mHits[index];').const_()

    c.method('mid','SearchHits', 'int index', 'return SearchHits(mPattern, mHits.mid(index));')

    c.method('read',cpp.void, {before: cpp.int, after: cpp.int},
    `for(int i=0;i<mHits.size();i++) {
        mHits[i].read(before, after);
    }`)

    c.method('clearCache', cpp.void, '', 
    `for(int i=0;i<mHits.size();i++) {
        mHits[i].clearCache();
    }
    `)

    c.method('clear',cpp.void,'','mHits.clear();')

    c.include('SearchHit')
    c.include('RegExp')
    //c.include('RegExpReplacement')
    c.include('QMetaType', true, true)
    c.metatype()
    c.write(src)
}


function SearchNameHits() {
    let c = new CppClass('SearchNameHits')
    let m = {
        pattern: 'RegExp',
        hits: 'QStringList'
    }
    c.constructor_()
    c.constructor_('const RegExp& pattern')

    for (let k in m) {
        c.member(mName(k), m[k])
    }
    
    c.method('append',cpp.void,'const SearchNameHits& hits', 
    `mPattern = hits.pattern(); 
    mHits.append(hits.hits());`)

    c.method('append',cpp.void, 'const QString& path',
    `mHits.append(path);`)

    c.method('clear',cpp.void,'','mHits.clear();')

    c.include('QMetaType', true, true)
    c.include('RegExp')
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
        replacement: 'RegExpReplacement',
        cacheFileList: cpp.bool,
    }
    c.constructor_()
    //c.constructor_(m)
    for (let k in m) {
        c.member(mName(k), m[k], {id: -1, cacheFileList: false, skipBinary: true}[k])
    }

    c.include('QMetaType', true, true)
    c.include('RegExp')
    c.include('RegExpPath')
    c.include('RegExpReplacement')
    c.metatype()
    c.write(src)
}

function SearchTab() {
    let c = new CppClass('SearchTab')
    c.inherits('QWidget')
    let m = {
        mode: 'Mode',
        params: 'SearchParams',
        hits: 'SearchHits',
        nameHits: 'SearchNameHits',
        updated: qt.QDateTime
    }
    
    let s = new CppSignature()

    c.constructor_(s.signature({parent: pointer(qt.QWidget)}, {parent: 0}), {'QWidget': 'parent', 'mUpdated': 'QDateTime::currentDateTime()'},
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
    mDisplayOptionsWidget = new DisplayOptionsWidget();
    mRenderer = new SearchResultRenderer();
    layout->addWidget(mTextBrowser);
    layout->addWidget(mDisplayOptionsWidget);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    setLayout(layout);
    mRenderer->setTab(this);
    `)

    c.method('append',cpp.void, 'const SearchHits& hits, const SearchNameHits& nameHits',
    `mUpdated = QDateTime::currentDateTime();
    if (hits.isEmpty()) {
        return;
    }
    int size = mHits.size();
    mHits.append(hits);
    mNameHits.append(nameHits);
    read();
    mRenderer->append(mHits.mid(size));
    `)

    c.method('read', cpp.void, '', 
    `DisplayOptions options = mDisplayOptionsWidget->options();
    mHits.read(options.linesBefore(), options.linesAfter());`)

    c.method('trigRerender', cpp.void, '', 'mDisplayOptionsWidget->trigChanged();')

    //c.constructor_(m)
    for (let k in m) {
        c.member(mName(k), m[k], {id: -1, mode: 'Mode::Search'}[k], {
                getter: ['params','hits','nameHits'].indexOf(k) < 0, 
                setter: ['mode'].indexOf(k) < 0,
                simpleTypes:['Mode']
        })
    }

    c.method('setMode',cpp.void,'Mode value','if (mMode == value) {return;} mMode = value; trigRerender();')

    c.member('mTextBrowser', 'QTextBrowser*')
    c.member('mDisplayOptionsWidget','DisplayOptionsWidget*')
    c.member('mRenderer','SearchResultRenderer*')

    c.method('params', 'SearchParams&', '', `return mParams;`)
    c.method('hits', 'SearchHits&','','return mHits;')
    c.method('nameHits','SearchNameHits&','','return mNameHits;')

    c.method('displayOptions', 'DisplayOptions', '', 'return mDisplayOptionsWidget->options();').const_()
    c.method('setDisplayOptions', cpp.void, 'const DisplayOptions& value','mDisplayOptionsWidget->setOptions(value);')

    //c.method('setPattern', cpp.void, 'const RegExp& value', 'mParams.setPattern(value);')

    c.method('replaceParams', 'ReplaceParams', 'bool renameFiles', 'return mRenderer->replaceParams(renameFiles);')

    c.method('toPlainText', qt.QString, '', 'return mTextBrowser->toPlainText();').const_()
    c.method('toHtml', qt.QString, '', 'return mTextBrowser->toHtml();').const_()

    c.include('SearchHits')
    c.include('SearchNameHits')
    c.include('SearchParams')
    c.include('Utils')
    c.include('Html')
    c.include('ColoredLine')
    c.include('Mode')
    c.include('DisplayOptionsWidget')
    c.include('SearchResultRenderer')

    c.qobject()
    c.write(src)
}

function ReplaceItem() {
    let c = new CppClass('ReplaceItem')

    let m = {
        line: cpp.int,
        before: qt.QString,
        after: qt.QString
    }
    
    c.constructor_()
    c.constructor_(m)
    for (let k in m) {
        c.member(mName(k), m[k])
    }

    

    c.include('QMetaType', true, true)
    c.metatype()
    c.write(src)
}

function ReplaceFile() {
    let c = new CppClass('ReplaceFile')
    let m = {
        path: qt.QString,
        items: 'QList<ReplaceItem>'
    }
    c.constructor_()
    c.constructor_('const QString& path')
    for (let k in m) {
        c.member(mName(k), m[k])
    }

    c.method('append',cpp.void,'const ReplaceItem& item', 'mItems.append(item);')

    c.method('size', cpp.int, '', 'return mItems.size();').const_()

    c.include('ReplaceItem')
    c.include('QMetaType', true, true)
    c.metatype()
    c.write(src)
}

function ReplaceParams() {
    let c = new CppClass('ReplaceParams')

    let m = {
        files: 'QList<ReplaceFile>',
        renameFiles: cpp.bool,
        renames: 'QList<QPair<QString,QString>>'
    }

    c.constructor_()
    c.constructor_(m)
    for (let k in m) {
        c.member(mName(k), m[k])
    }

    c.method('append', cpp.void, 'const ReplaceFile& file', 'mFiles.append(file);')

    c.method('size', cpp.int, '', 'return mFiles.size();').const_()

    c.include('ReplaceFile')
    c.include('QMetaType', true, true)
    c.metatype()
    c.write(src)
}

function SearchData() {
    let c = new CppClass('SearchData')
    let m = {
        files: qt.QStringList,
        filesFiltered: cpp.int,
        dirsFiltered: cpp.int
    }
    c.constructor_()
    c.constructor_(m)
    for (let k in m) {
        c.member(mName(k), m[k])
    }
    c.member('mFilesComplete',cpp.int, 0)
    c.method('file',qt.QString,'int i','return mFiles[i];').const_()
    c.method('filesSize',cpp.int,'','return mFiles.size();').const_()
    c.write(src)
}

function RegExpReplacement() {
    let c = new CppClass('RegExpReplacement')

    let m = {
        pattern: qt.QString,
        preserveCase: cpp.bool,
        renameFiles: cpp.bool
    }
    c.constructor_()
    c.constructor_(m)
    for (let k in m) {
        c.member(mName(k), m[k], {preserveCase: false, renameFiles: true}[k])
    }

    c.include('QMetaType', true, true)
    c.metatype()
    c.write(src)
}

function DisplayOptions() {
    let c = new CppClass('DisplayOptions')
    let m = {
        linesBefore: cpp.int,
        linesAfter: cpp.int,
        fileName: cpp.bool,
        lineNumber: cpp.bool,
        wholeLine: cpp.bool
    }
    c.constructor_()
    c.constructor_(m)
    for (let k in m) {
        c.member(mName(k), m[k], {linesBefore: 0, linesAfter: 0, fileName: true, lineNumber: true, wholeLine: true}[k])
    }
    c.write(src)
}

function just(m, ...keys) {
    let r = {}
    keys.forEach(k => r[k] = m[k])
    return r;
}

/*
function CountFilesParams() {
    let c = new CppClass('CountFilesParams')
    let m = {
        path: qt.QString,
        filter: 'RegExpPath',
        cacheFileList: cpp.bool,
        total: cpp.int,
        filtered: cpp.int,
        notBinary: cpp.bool
    }
    c.constructor_()
    c.constructor_(just(m, ['path','filter','notBinary']))
    c.constructor_(m)
    for (let k in m) {
        c.member(mName(k), m[k], {total: -1, filtered: -1, cacheFileList: true}[k])
    }
    c.include('QMetaType', true, true)
    c.include('RegExpPath')
    c.metatype()
    c.write(src)
}*/

function GetListingParams() {
    let c = new CppClass('GetListingParams')
    let m = {
        path: qt.QString,
        cacheFileList: cpp.bool
    }
    for (let k in m) {
        c.member(mName(k), m[k], {cacheFileList: false}[k])
    }
    c.constructor_()
    c.constructor_(m)
    c.include('QMetaType', true, true)
    c.metatype()
    c.write(src)
}

/*
function FileCountItem() {
    let c = new CppClass('FileCountItem')
    let m = {
        path: qt.QString,
        filter: 'RegExpPath',
        notBinary: cpp.bool,
        total: cpp.int,
        filtered: cpp.int
    }


    c.write(src)
}*/

function CountFilesParams() {
    let c = new CppClass('CountFilesParams')
    let m = {
        path: qt.QString,
        filter: 'RegExpPath',
        total: cpp.int,
        filtered: cpp.int
    }
    c.constructor_()
    c.constructor_(m)
    for (let k in m) {
        c.member(mName(k), m[k], {total: -1, filtered: -1}[k])
    
    }
    c.metatype()
    c.include('QMetaType', true, true)
    c.include('RegExpPath')
    c.write(src)
}

function Editor() {

    let opt = {simpleTypes: ['Editor::Type']}

    let c = new CppClass('Editor', opt)
    let m = {
        exts: qt.QString,
        app: qt.QString,
        type: 'Editor::Type',
        exp: 'QRegularExpression'
    }

    for (let k in m) {
        c.member(mName(k), m[k], {type: 'Type::NotSpecified'}[k], {exts: {setter: false}}[k])
    }

    c.definition(`
    enum class Type {
        NotSpecified,
        VsCode,
        QtCreator,
        Geany
    };
    `,sc.CLASS)

    c.method('name', qt.QString, '',
    `static QMap<Editor::Type,QString> names = {
        {Type::VsCode, "VsCode"},
        {Type::QtCreator, "QtCreator"},
        {Type::Geany, "Geany"},
    }; return names.value(mType);`).const_()

    c.method('setExts',cpp.void, 'const QString& value',
    `
    mExts = value;
    mExp = QRegularExpression("^(" + mExts + ")$");
    `)

    c.method('isNull', cpp.bool, '', 'return mApp.isEmpty();').const_()

    c.method('matches',cpp.bool,'const QString& ext', `return mExp.match(ext).hasMatch();`).const_()

    c.method('fromJson', 'Editor', 'const QJsonObject& json',
    `return Editor(json.value("exts").toString(), json.value("app").toString());
    `).static()

    c.method('toJson','QJsonObject', '', 
    `QJsonObject json;
    json["exts"] = mExts;
    json["app"] = mApp;
    return json;
    `).const_()

    c.constructor_()

    let s = new CppSignature(opt)

    c.constructor_(s.signature(just(m, 'exts','app', 'type'), {type: 'Type::NotSpecified'}),{},'setExts(exts);')
    //c.constructor_('const QString& exts, const QString& app',{},'setExts(exts);')
    
    c.write(src)
}

function RenameParams() {

    let c = new CppClass('RenameParams')
    let m = {
        renames: 'QList<QPair<QString,QString>>'
    }
    c.constructor_()
    c.constructor_(m)
    for (let k in m) {
        c.member(mName(k), m[k])
    }
    c.metatype()
    c.include('QMetaType', true, true)
    c.write(src)
}

function ReplacedParams() {
    let c = new CppClass('ReplacedParams')
    let m = {
        countFiles: cpp.int,
        countLines: cpp.int,
        renames: 'QList<QPair<QString,QString>>'
    }
    c.constructor_()
    c.constructor_(m)
    for (let k in m) {
        c.member(mName(k), m[k])
    }
    c.metatype()
    c.include('QMetaType', true, true)
    c.write(src)
}

// todo mugicpp metatype: add include QMetaType
// todo mugicpp incude class Foo if ref or pointer
// todo mugicpp canSpawn
// todo mugicpp QRegularExpression,QJsonObject, qint64, qint8, etc

SearchHit()
SearchHits()
SearchNameHits()
SearchParams()
ReplaceItem()
ReplaceParams()
SearchTab()
SearchData()
RegExpReplacement()
DisplayOptions()
ReplaceFile()
CountFilesParams()
GetListingParams()
Editor()
RenameParams()
ReplacedParams()
