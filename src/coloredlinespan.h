#ifndef COLOREDLINESPAN_H
#define COLOREDLINESPAN_H
class ColoredLineSpan {
public:
    ColoredLineSpan(int start = 0, int end = 0, int foreground = 0, int background = 0);
    int start() const;
    int end() const;
    int foreground() const;
    int background() const;
    int length() const;

protected:
    int mStart;
    int mEnd;
    int mForeground;
    int mBackground;
};
#endif // COLOREDLINESPAN_H
