#ifndef LINECONTEXT_H
#define LINECONTEXT_H

#include <QTextStream>

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

protected:

    bool mInit;
    QList<LineContextItem> mContext;

    void parseCpp(const QString &path);
    void parsePy(const QString &path);
};

#endif // LINECONTEXT_H
