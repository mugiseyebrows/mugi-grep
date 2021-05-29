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
#include "countfilesparams.h"
#include "getlistingparams.h"
#include "searchresultrenderer.h"
#include "replacedparams.h"

#include <QStyleFactory>
#include "settings.h"
#include "stylehelper.h"
#include "renameparams.h"
#include "searchnamehits.h"
#include "style.h"
#include "linecontext.h"

int main(int argc, char *argv[])
{
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    qRegisterMetaType<RegExp>();
    qRegisterMetaType<RegExpPath>();
    qRegisterMetaType<RegExpReplacement>();
    qRegisterMetaType<SearchParams>();
    qRegisterMetaType<ReplaceParams>();
    qRegisterMetaType<SearchHits>();
    qRegisterMetaType<CountFilesParams>();
    qRegisterMetaType<GetListingParams>();
    qRegisterMetaType<ReplacedParams>();
    qRegisterMetaType<RenameParams>();
    qRegisterMetaType<SearchNameHits>();

    QApplication a(argc, argv);
    a.setApplicationName(APP_NAME);
    a.setApplicationVersion(APP_VERSION);

    Settings settings;

    QStringList keys = QStyleFactory::keys();

    QStyle* fusion = QStyleFactory::create("Fusion");
    Style* style_ = new Style(fusion);

    qApp->setStyle(style_);

    QString style = settings.style();
    if (style == "dark") {
        StyleHelper::setDarkStyle();
    } else {
        StyleHelper::setLightStyle();
    }

    //
    /*a.setStyle("Windows");
    QStringList keys = QStyleFactory::keys();
    qDebug() << keys[0];*/
    //RegExpPath::test();
    //RegExp::test();
    //SearchResultRenderer::testDoZebra();

    MainWindow w(&settings);
    w.show();

    if (a.applicationDirPath().endsWith("debug")) {
        LineContext::testParseCpp();
    }

    //SearchCache::testTokenize();

    return a.exec();
}
