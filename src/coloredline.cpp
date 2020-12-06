#include "coloredline.h"

ColoredLine::ColoredLine(const QString& string, const QList<int>& foreground,
                         const QList<int>& background)
    : mString(string), mForeground(foreground), mBackground(background) {
    while (mForeground.size() < mString.size()) {
        mForeground << 0;
    }
    while (mBackground.size() < mString.size()) {
        mBackground << 0;
    }
}

QString ColoredLine::string() const {
    return mString;
}

QList<int> ColoredLine::foreground() const {
    return mForeground;
}

QList<int> ColoredLine::background() const {
    return mBackground;
}

void ColoredLine::paintForeground(int start, int end, int color) {
    paint(start, end, color, mForeground);
}

void ColoredLine::paintForeground(int color)
{
    paint(0, mForeground.size(), color, mForeground);
}

void ColoredLine::paintBackground(int start, int end, int color) {
    paint(start, end, color, mBackground);
}

void ColoredLine::paintBackground(int color)
{
    paint(0, mBackground.size(), color, mBackground);
}

void ColoredLine::paint(int start, int end, int color, QList<int>& dest) {
    for (int i = start; i < end; i++) {
        dest[i] = color;
    }
}

ColoredLine ColoredLine::mid(int pos, int length) const {
    return ColoredLine(mString.mid(pos, length), mForeground.mid(pos, length),
                       mBackground.mid(pos, length));
}

QList<ColoredLineSpan> ColoredLine::spans() const {
    QList<ColoredLineSpan> result;
    if (mForeground.isEmpty()) {
        return result;
    }
    int prevIndex = 0;
    int prevForeground = mForeground[0];
    int prevBackground = mBackground[0];
    for (int i = 0; i < mForeground.size(); i++) {
        if (prevForeground != mForeground[i] || prevBackground != mBackground[i]) {
            result << ColoredLineSpan(prevIndex, i, prevForeground, prevBackground);
            prevForeground = mForeground[i];
            prevBackground = mBackground[i];
            prevIndex = i;
        }
    }
    result << ColoredLineSpan(prevIndex, mForeground.size(), prevForeground, prevBackground);
    return result;
}
