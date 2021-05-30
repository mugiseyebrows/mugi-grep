#ifndef COLORS_H
#define COLORS_H

#include <QApplication>
#include <QPalette>

class Colors {
public:
    Colors(bool zebra);

    QString baseColor() const {
        return palette.color(QPalette::Base).name();
    }

    QString alternateBaseColor() const {
        return palette.color(QPalette::AlternateBase).name();
    }

    QString anchorColor() const {
        return palette.color(QPalette::Link).name();
    }

    QString separatorColor() const {
        return darkStyle ? "#ffffff" : "#000000";
    }

    QString linenumberColor() const {
        return darkStyle ? "#FFA95E" : "#FFA95E";
    }

    QString contextColor() const {
        return darkStyle ? "#5EEEFF" : "#00E5FF";
    }

    bool darkStyle;
    QPalette palette;
    QStringList backgroundColors;
    QStringList grayZebraColors;
    QString redColor;
    QString highlightedRedColor;
    QString greenColor;
    QString highlightedGreenColor;
};


#endif // COLORS_H
