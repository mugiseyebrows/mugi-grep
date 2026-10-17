#include <QApplication>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <cstdio>
#include "widget/mainwindow.h"


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

#define APP_NAME "mugi-grep"

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
    a.setApplicationVersion(VERSION);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("directory", "Folder to search", "[directory]");
    parser.process(a);
    const QStringList arguments = parser.positionalArguments();
    if (arguments.size() > 1 ||
        (!arguments.isEmpty() && !QFileInfo(arguments.first()).isDir())) {
        std::fprintf(stderr, "Expected one existing directory.\n");
        return 1;
    }

    Settings settings;
    if (!arguments.isEmpty()) {
        settings.setSessions(QJsonArray{QJsonObject{
            {"path", QFileInfo(arguments.first()).absoluteFilePath()}
        }});
    }

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
        LineContext::testParsePy();
    }

    //SearchCache::testTokenize();

    return a.exec();
}
