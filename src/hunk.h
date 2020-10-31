#ifndef HUNK_H
#define HUNK_H
#include <QString>
#include <QStringList>

#include "htmldivs.h"

class Hunk {
public:
    Hunk(const QString& subjBackgroundColor, const QString& replBackgroundColor);
    int line() const;
    QStringList lines() const;
    int count() const;
    bool isEmpty() const;
    void replace(int line, const QString& subj, const QString& repl);
    void context(int line, const QString& context);
    QStringList value();

protected:

    void flushRepl();
    void flushContext();

    HtmlDivs mSubj;
    HtmlDivs mRepl;
    HtmlDivs mContext;
    QString mSubjBackgroundColor;
    QString mReplBackgroundColor;
    int mLine;
    int mCount;
    QStringList mLines;
};
#endif // HUNK_H
