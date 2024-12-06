#ifndef PARSEPYTHON_H
#define PARSEPYTHON_H

#include "linecontext.h"
#include <QMap>

class PythonSymbol {
public:
    enum Type {
        Class,
        Method,
    };

    PythonSymbol() {}

    PythonSymbol(Type type, const QString& name, const QString& shortName, int begin, int end, int indent);

    bool contains(int line) const;

    Type type;
    QString name;
    QString shortName;
    int begin;
    int end;
    int indent;

};

class PythonReader {
public:
    enum class State {
        Code,
        SignleQuoteStr,
        DoubleQuoteStr,
        HeredocStr,
        Comment
    };

    PythonReader(QTextStream& stream);

    QPair<int,QString> read();

    QTextStream& stream;
    int lineNumber;

};


class PythonHelper {
public:
    void startClass(int lineNumber, int indent, const QString& name);

    void startMethod(int lineNumber, int indent, const QString& name, const QString& shortName);

    void flush(int indent, int lineNumber);

    QList<LineContextItem> context() const;

    QMap<int, PythonSymbol> incomplete;
    QList<PythonSymbol> complete;

};

class ParsePython
{
public:
    ParsePython();

    static QList<LineContextItem> parse(const QString &path);
};

#endif // PARSEPYTHON_H
