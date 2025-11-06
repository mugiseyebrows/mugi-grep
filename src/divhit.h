#ifndef DIVHIT_H
#define DIVHIT_H

#include <QStringList>
#include "coloredline.h"
class Colors;

class LineHit {
public:
    LineHit(int lineNumber, const QStringList& spans, bool matched) : lineNumber(lineNumber), spans(spans), matched(matched) {

    }
    int lineNumber;
    QStringList spans;
    bool matched;
};

QStringList fileNameLineNumber(const Colors& colors, bool showFileName, bool showLineNumber,
                               const QString& relativePath, const QString& href,
                               int lineNumber, const QString& separator);

QStringList fileNameLineNumberContext(const QString& color,
                                      bool showFileName, bool showLineNumber,
                                      const QString& relativePath,
                                      const QString& href,
                                      int lineNumber);

QString fileHref(const QString& path, int lineNumber);

class DivHit {
public:

    DivHit() {

    }

    DivHit(const QString absolutePath, const  QString relativePath)
        : absolutePath(absolutePath), relativePath(relativePath) {

    }

    QList<LineHit> lines;
    QString absolutePath;
    QString relativePath;
    QString backgroundColor;

    bool isNull() const {
        return absolutePath.isEmpty();
    }

    void append(const LineHit& hit) {
        lines.append(hit);
    }

    QString render(const Colors& colors, bool showFileName, bool showLineNumber) const;
};

class DivHit2 {
public:
    DivHit2() {

    }

    DivHit2(const QString absolutePath, const  QString relativePath)
        : absolutePath(absolutePath), relativePath(relativePath) {

    }

    QString render(const Colors& colors, bool showFileName, bool showLineNumber) const;

    //void setSpans(const QStringList& spans);

    void setSpans(const QList<QStringList>& spans) {
        mSpans = spans;
    }

    QString backgroundColor;

protected:
    QString absolutePath;
    QString relativePath;
    //QStringList mSpans;
    //QList<QList<ColoredLineSpan>> mSpans;
    QList<QStringList> mSpans;

};


#endif // DIVHIT_H
