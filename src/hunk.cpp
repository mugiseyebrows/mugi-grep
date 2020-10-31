#include "hunk.h"

Hunk::Hunk(const QString& subjBackgroundColor, const QString& replBackgroundColor) :
    mSubjBackgroundColor(subjBackgroundColor), mReplBackgroundColor(replBackgroundColor), mLine(-1), mCount(0) {
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
    return mLines.isEmpty() && mSubj.isEmpty() && mContext.isEmpty();
}

void Hunk::replace(int line, const QString& subj, const QString& repl) {
    flushContext();
    mSubj.append(subj,mSubjBackgroundColor);
    mRepl.append(repl,mReplBackgroundColor);
    if (mLine < 0)
        mLine = line;
    mCount++;
}

void Hunk::context(int line, const QString& context) {
    flushRepl();
    mContext.append(context, QString());
    if (mLine < 0)
        mLine = line;
    //mCount++;
}

void Hunk::flushContext() {
    if (mContext.isEmpty()) {
        return;
    }
    mContext.close();
    mLines << mContext.divs().join("");
    mContext.clear();
}

void Hunk::flushRepl() {
    if (mSubj.isEmpty()) {
        return;
    }
    mSubj.close();
    mRepl.close();
    mLines << mSubj.divs().join("");
    mLines << mRepl.divs().join("");
    mSubj.clear();
    mRepl.clear();
}

QStringList Hunk::value() {
    flushRepl();
    flushContext();
    return mLines;
}
