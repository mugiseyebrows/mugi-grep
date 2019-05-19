#ifndef REPLACEMENTLINE_H
#define REPLACEMENTLINE_H

#include <QString>

class ReplacementLine {
public:
    ReplacementLine();
    ReplacementLine(int line, const QString& oldLine, const QString& newLine);
    int line() const;
    QString oldLine() const;
    QString newLine() const;

protected:
    int mLine;
    QString mOldLine;
    QString mNewLine;
};


#endif // REPLACEMENTLINE_H
