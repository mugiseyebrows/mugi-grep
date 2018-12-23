#ifndef HTML_H
#define HTML_H

#include <QString>

namespace Html
{

    QString span(const QString &text_, const QString &color);

    QString anchor(const QString &text, const QString& path, const QString& color);

}

#endif // HTML_H
