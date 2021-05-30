#include "linecontext.h"

#include <QFileInfo>
#include <QByteArray>
#include <QDebug>
#include <QElapsedTimer>
#include "parse/parsepython.h"
#include "parse/parsecpp.h"

LineContext::LineContext() : mInit(false)
{

}

void LineContext::parseCpp(const QString &path) {

    mContext = ParseCpp::parse(path);
}

void LineContext::parsePython(const QString &path) {
    mContext = ParsePython::parse(path);
}

void LineContext::init(const QString &path)
{
    if (mInit) {
        return;
    }
    mInit = true;

    QFileInfo info(path);

    QStringList cpp = {"cpp", "cxx", "h", "hpp"};
    QStringList py = {"py"};

    QString ext = info.suffix();

    if (cpp.contains(ext)) {
        parseCpp(path);
    }
    if (py.contains(ext)) {
        parsePython(path);
    }
}

LineContextItem LineContext::context(int line) const
{
     for(int i=0;i<mContext.size();i++) {
         if (mContext[i].contains(line)) {
             return mContext[i];
         }
     }
     return LineContextItem();
}

void LineContext::dump()
{
    for(int i=0;i<mContext.size();i++) {
        const LineContextItem& item = mContext[i];
        qDebug() << "ITEM" << item.name << (item.begin + 1) << (item.end + 1);
    }
}

void LineContext::testParsePy()
{
    auto PARSE_PY_FILE = qgetenv("PARSE_PY_FILE");
    if (PARSE_PY_FILE.isNull()) {
        return;
    }
    LineContext context;
    QElapsedTimer timer;
    timer.start();
    context.init(QString::fromUtf8(PARSE_PY_FILE));
    qDebug() << "context.init py" << timer.elapsed() << "ms";
    context.dump();
}

void LineContext::testParseCpp()
{
    auto PARSE_CPP_FILE = qgetenv("PARSE_CPP_FILE");
    if (PARSE_CPP_FILE.isNull()) {
        return;
    }
    LineContext context;
    QElapsedTimer timer;
    timer.start();
    context.init(QString::fromUtf8(PARSE_CPP_FILE));
    qDebug() << "context.init cpp" << timer.elapsed() << "ms";
    context.dump();
}
