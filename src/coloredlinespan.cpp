#include "coloredlinespan.h"

ColoredLineSpan::ColoredLineSpan(int start, int end, int foreground, int background)
    : mStart(start), mEnd(end), mForeground(foreground), mBackground(background) {
}

int ColoredLineSpan::start() const {
    return mStart;
}

int ColoredLineSpan::end() const {
    return mEnd;
}

int ColoredLineSpan::foreground() const {
    return mForeground;
}

int ColoredLineSpan::background() const {
    return mBackground;
}

int ColoredLineSpan::length() const {
    return mEnd - mStart;
}
