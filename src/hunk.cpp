#include "hunk.h"

Hunk::Hunk() : mLine(-1), mCount(0) {
}

int Hunk::line() const {
    return mLine;
}

QStringList Hunk::lines() const {
    return mLines;
}

int Hunk::count() const {
    return mCount;
}

bool Hunk::isEmpty() const {
    return mLines.isEmpty() && mSubj.isEmpty();
}

void Hunk::replace(int line, const QString& subj, const QString& repl) {
    mSubj << subj;
    mRepl << repl;
    if (mLine < 0)
        mLine = line;
    mCount++;
}

void Hunk::context(int line, const QString& context) {
    flush();
    mLines << context;
    if (mLine < 0)
        mLine = line;
    mCount++;
}

void Hunk::flush() {
    if (mSubj.isEmpty())
        return;
    mLines << mSubj << mRepl;
    mSubj.clear();
    mRepl.clear();
}

QStringList Hunk::value() {
    flush();
    QStringList res;
    return mLines;
}
