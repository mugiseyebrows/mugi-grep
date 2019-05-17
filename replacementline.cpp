#include "replacementline.h"

ReplacementLine::ReplacementLine() {

}

ReplacementLine::ReplacementLine(int line, const QString &oldLine, const QString &newLine) : mLine(line), mOldLine(oldLine), mNewLine(newLine) {

}

int ReplacementLine::line() const {
    return mLine;
}

QString ReplacementLine::oldLine() const {
    return mOldLine;
}

QString ReplacementLine::newLine() const {
    return mNewLine;
}
