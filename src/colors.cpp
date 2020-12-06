#include "colors.h"

bool isDarker(const QColor& color1, const QColor& color2) {
    double v1 = color1.valueF();
    double v2 = color2.valueF();
    return v1 < v2;
}

Colors::Colors(bool zebra) {
    palette = qApp->palette();
    darkStyle = isDarker(palette.color(QPalette::Base), QColor(128,128,128));

    if (darkStyle) {
        backgroundColors = QStringList {QString(), "#09194f", "#70041b", "#560100", "#151350", "#3f1440", "#113302"};
    } else {
        backgroundColors = QStringList {QString(), "#ffdfba", "#baffc9", "#bae1ff", "#ffffba", "#ffb3ba"};
    }

    if (zebra) {
        grayZebraColors = QStringList {baseColor(), alternateBaseColor()};
    } else {
        grayZebraColors = QStringList {QString(), QString()};
    }

    redColor = darkStyle ? "#7A0311" : "#FDB8C0";
    highlightedRedColor = darkStyle ? "#B7051A" : "#FC9BA6";
    greenColor = darkStyle ? "#004412" : "#D9FFE3";
    highlightedGreenColor = darkStyle ? "#008824" : "#98FFB3";

}
