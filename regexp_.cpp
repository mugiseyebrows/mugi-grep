#if 0

#include "regexp.h"



RegExp::RegExp()
{
}

RegExp::RegExp(const QString inc, const QString exc, bool case_)
{
}

QRegExp RegExp::createInclude(const QString& include, const QString& exclude, int syntax, int mode, bool matchCase)
{
    return QRegExp(include,matchCase ? Qt::CaseSensitive : Qt::CaseInsensitive, (QRegExp::PatternSyntax)syntax);
}

QRegExp RegExp::createExclude(const QString& include, const QString& exclude, int syntax, int mode, bool matchCase)
{
    return QRegExp(exclude,matchCase ? Qt::CaseSensitive : Qt::CaseInsensitive, (QRegExp::PatternSyntax)syntax);
}

bool RegExp::includeIsValid(const QRegExp &exp, int mode)
{
    if (mode == Include || mode == IncludeAndExclude)
        return exp.isValid();
    return true;
}

bool RegExp::excludeIsValid(const QRegExp &exp, int mode)
{
    if (mode == Exclude || mode == IncludeAndExclude)
        return exp.isValid();
    return true;
}

bool RegExp::matches(const QString& line, QRegExp &include, QRegExp &exclude, int mode)
{
    switch(mode) {
    case RegExp::Include:
        return line.indexOf(include) > -1;
    case RegExp::Exclude:
        return line.indexOf(exclude) < 0;
    case RegExp::IncludeAndExclude:
        return line.indexOf(include) > -1 && line.indexOf(exclude) < 0;
    }
    return false;
}

#endif
