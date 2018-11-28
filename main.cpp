#include <QApplication>
#include "widget/mainwindow.h"

#include <QTextCodec>

#include "regexp.h"
#include "regexppath.h"


int main(int argc, char *argv[])
{
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    qRegisterMetaType<RegExp>();
    qRegisterMetaType<RegExpPath>();

    QApplication a(argc, argv);

    //RegExpPath::test();
    //RegExp::test();

    MainWindow w;
    w.show();
    
    return a.exec();
}
