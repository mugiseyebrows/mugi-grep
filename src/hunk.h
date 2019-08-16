#ifndef HUNK_H
#define HUNK_H
#include <QString>
#include <QStringList>

class Hunk {
public:
    Hunk();
    int line() const;
    QStringList lines() const;
    int count() const;
    bool isEmpty() const;
    void replace(int line, const QString& subj, const QString& repl);
    void context(int line, const QString& context);
    QStringList value();

protected:
    void flush();
    int mLine;
    QStringList mLines;
    int mCount;
    QStringList mSubj;
    QStringList mRepl;
};
#endif // HUNK_H
