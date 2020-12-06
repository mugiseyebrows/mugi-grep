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
        return darkStyle ? "#74c69d" : "#2d6a4f";
    }

    QString linenumberColor() const {
        return darkStyle ? "#e85d04" : "#dc2f02";
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
