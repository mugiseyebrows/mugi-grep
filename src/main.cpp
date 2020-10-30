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
    qRegisterMetaType<CountFilesParams>();
    qRegisterMetaType<GetListingParams>();

    QApplication a(argc, argv);
    a.setApplicationName(APP_NAME);
    a.setApplicationVersion(APP_VERSION);

    qApp->setStyle(QStyleFactory::create("Fusion"));

    QPalette newPalette;
    newPalette.setColor(QPalette::Window,          QColor( 37,  37,  37));
    newPalette.setColor(QPalette::WindowText,      QColor(212, 212, 212));
    newPalette.setColor(QPalette::Base,            QColor( 60,  60,  60));
    newPalette.setColor(QPalette::AlternateBase,   QColor( 45,  45,  45));

    newPalette.setColor(QPalette::Text,            QColor(212, 212, 212));
    newPalette.setColor(QPalette::Button,          QColor( 45,  45,  45));
    newPalette.setColor(QPalette::ButtonText,      QColor(212, 212, 212));
    newPalette.setColor(QPalette::BrightText,      QColor(240, 240, 240));
    newPalette.setColor(QPalette::Highlight,       QColor( 38,  79, 120));
    newPalette.setColor(QPalette::HighlightedText, QColor(240, 240, 240));

    newPalette.setColor(QPalette::Light,           QColor( 60,  60,  60));
    newPalette.setColor(QPalette::Midlight,        QColor( 52,  52,  52));
    newPalette.setColor(QPalette::Dark,            QColor( 30,  30,  30) );
    newPalette.setColor(QPalette::Mid,             QColor( 37,  37,  37));
    newPalette.setColor(QPalette::Shadow,          QColor( 0,    0,   0));

    newPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));

    //qApp->setPalette(newPalette);

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
