#include <QApplication>
#include "widget/mainwindow.h"

#include <QTextCodec>

#include "regexp.h"
#include "regexppath.h"
#include "version.h"

#include "searchcache.h"
#include "searchparams.h"
#include "searchhits.h"
#include "regexpreplacement.h"
#include "replaceparams.h"

#include <QStyleFactory>

int main(int argc, char *argv[])
{
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    qRegisterMetaType<RegExp>();
    qRegisterMetaType<RegExpPath>();
    qRegisterMetaType<RegExpReplacement>();
    qRegisterMetaType<SearchParams>();
    qRegisterMetaType<ReplaceParams>();
    qRegisterMetaType<SearchHits>();

    QApplication a(argc, argv);
    a.setApplicationName(APP_NAME);
    a.setApplicationVersion(APP_VERSION);
    /*a.setStyle("Windows");
    QStringList keys = QStyleFactory::keys();
    qDebug() << keys[0];*/

    //RegExpPath::test();
    //RegExp::test();

    MainWindow w;
    w.show();

    //SearchCache::testTokenize();
    
    return a.exec();
}
