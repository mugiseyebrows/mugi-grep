#ifndef REGEXP_H
#define REGEXP_H

#if 0

#include <QRegExp>



class RegExp
{
public:
    enum RegExpMode {
        Include,
        Exclude,
        IncludeAndExclude
    };

    RegExp();

    static bool includeIsValid(const QRegExp& exp, int mode);
    static bool excludeIsValid(const QRegExp& exp, int mode);

    static bool matches(const QString &line, QRegExp& include, QRegExp& exclude, int mode);

    static QRegExp createExclude(const QString &include, const QString &exclude, int syntax, int mode, bool matchCase);
    static QRegExp createInclude(const QString &include, const QString &exclude, int syntax, int mode, bool matchCase);
};

#endif

#endif // REGEXP_H
