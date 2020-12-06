#ifndef COLOREDLINE_H
#define COLOREDLINE_H
#include "coloredlinespan.h"
#include <QList>
#include <QString>

class ColoredLine {
public:
    ColoredLine(const QString& string = QString(), const QList<int>& foreground = {},
                const QList<int>& background = {});
    QString string() const;
    QList<int> foreground() const;
    QList<int> background() const;
    void paintForeground(int start, int end, int color);
    void paintForeground(int color);
    void paintBackground(int start, int end, int color);
    void paintBackground(int color);
    ColoredLine mid(int pos, int length = -1) const;
    QList<ColoredLineSpan> spans() const;

protected:
    static void paint(int start, int end, int color, QList<int>& dest);
    QString mString;
    QList<int> mForeground;
    QList<int> mBackground;
};
#endif // COLOREDLINE_H
