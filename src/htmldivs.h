#ifndef HTMLDIVS_H
#define HTMLDIVS_H

#include <QStringList>

class HtmlDivs {

public:
    HtmlDivs() {

    }

    void append(const QString& line, const QString& color);

    void close();

    QStringList divs() const;

protected:

    QStringList mLines;
    QStringList mDivs;
    QString mBackgroundColor;

};


#endif // HTMLDIVS_H
