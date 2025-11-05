#include "tst_mugigrep.h"
#include <QTest>

#include "../src/mugigrep.h"
#include "../src/searchhit.h"
#include "../src/regexppair.h"

tst_MugiGrep::tst_MugiGrep(QObject *parent) :
    QObject(parent)
{
}

void tst_MugiGrep::initTestCase()
{
    
}

void tst_MugiGrep::cleanupTestCase()
{
    
}

// grep -i linenumber /d/dev/mugi-grep/src/searchresultrenderer.cpp | wc -l
// 43
// grep -i linenumber /d/dev/mugi-grep/src/searchresultrenderer.cpp | grep -iv separator | wc -l
// 37

void tst_MugiGrep::searchSingleline()
{

    // todo rel path
    // 27 kb
    QString path = "D:/dev/mugi-grep/src/searchresultrenderer.cpp";
    QString relPath;

    bool searchBinary = false;

    QString inc = "linenumber"; // 57
    QString exc = "separator"; // 23

    RegExpPair pattern(inc, exc, false, false, false);
    qint64 bytesRead;

    // read whole file into memory
    qint64 memLim = 100000;
    qint64 bufSize = 10000;
    SearchHit hit1 = ::searchSingleline(path, relPath, pattern, searchBinary, memLim, bufSize, &bytesRead);

    // read block by block
    memLim = 10000;
    for(bufSize=500;bufSize<5000;bufSize += 500) {
        SearchHit hit2 = ::searchSingleline(path, relPath, pattern, searchBinary, memLim, bufSize, &bytesRead);
        QCOMPARE(hit1.hits(), hit2.hits());
    }
    QCOMPARE(hit1.hits().size(), 37);
}

