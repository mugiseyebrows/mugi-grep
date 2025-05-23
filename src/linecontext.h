#ifndef LINECONTEXT_H
#define LINECONTEXT_H

#include <QTextStream>
#include <QString>
#include <QList>

class LineContextItem {
public:
    LineContextItem() : begin(-1), end(-1) {}
    LineContextItem(const QString& name, const QString& shortName, int begin, int end) : name(name), shortName(shortName), begin(begin), end(end) {

    }
    bool contains(int line) const {
        return begin < line && line < end;
    }

    bool isNull() {
        return begin < 0;
    }

    QString name;
    QString shortName;
    int begin;
    int end;
};

class LineContext
{
public:
    LineContext();

    void init(const QString& path);

    LineContextItem context(int line) const;

    void dump();

    static void testParseCpp();

    static void testParsePy();
protected:

    bool mInit;
    QList<LineContextItem> mContext;

    void parseCpp(const QString &path);
    void parsePython(const QString &path);
};

#endif // LINECONTEXT_H
