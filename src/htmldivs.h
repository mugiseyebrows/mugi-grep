#ifndef HTMLDIVS_H
#define HTMLDIVS_H

#include <QStringList>

class HtmlDivs {

public:
    HtmlDivs() {

    }

    static QString div(const QString& text, const QString& backgroundColor = QString());

    void append(const QString& line, const QString& color = QString());

    void close();

    QStringList divs() const;

    bool isEmpty() const;

    void clear();

    void appendContext(const QString &name);
protected:

    QStringList mLines;
    QStringList mDivs;
    QString mBackgroundColor;

};


#endif // HTMLDIVS_H
