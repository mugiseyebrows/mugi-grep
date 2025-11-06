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

    qint64 bufSize = 30000;
    SearchHit hit1 = ::searchSingleline(path, relPath, pattern, searchBinary, bufSize, &bytesRead);

    for(bufSize=1000;bufSize<10000;bufSize += 1000) {
        SearchHit hit2 = ::searchSingleline(path, relPath, pattern, searchBinary, bufSize, &bytesRead);

        /*QList<int> hits = hit2.hits();
        hits.append(4);
        hit2.setHits(hits);*/

        QCOMPARE(hit1, hit2);
    }
    QVERIFY(hit1.hits().contains(92-1));
    QCOMPARE(hit1.hits().size(), 37);
}

void tst_MugiGrep::searchMultiline() {

    // todo rel path
    // 27 kb
    QString path = "D:/dev/mugi-grep/src/searchresultrenderer.cpp";
    QString relPath;

    bool searchBinary = false;

    QString inc = "color.*?return";
    QString exc = {};

    RegExpPair pattern(inc, exc, false, true, true);
    qint64 bytesRead;

    qint64 bufSize = 30000;
    SearchHit hit1 = ::searchMultiline(path, relPath, pattern, searchBinary, bufSize, &bytesRead);

    for(bufSize=5000;bufSize<30000;bufSize += 5000) {
        SearchHit hit2 = ::searchMultiline(path, relPath, pattern, searchBinary, bufSize, &bytesRead);

        /*qDebug() << hit1.hits();
        qDebug() << hit2.hits();*/

        QCOMPARE(hit1, hit2);
    }
    QVERIFY(hit1.hits().contains(640));

}

