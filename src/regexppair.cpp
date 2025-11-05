#include "regexppair.h"

#include <QStringList>
#include <QDebug>

RegExpPair::RegExpPair()
{
    init({}, {}, false, false, false);
}

RegExpPair::RegExpPair(const QString &inc, const QString &exc, bool caseSensitive, bool dotAll, bool multiline)
{
    init(inc,exc,caseSensitive,dotAll,multiline);
}
/*
RegExpPair::RegExpPair(const QStringList &regExps, bool case_)
{
    init(regExps[0],regExps[1],case_);
}*/

RegExpPair::RegExpPair(const QVariantMap &data)
{
    deserealize(data);
}

bool RegExpPair::isEmpty() const
{
    return mInclude.isEmpty() && mExclude.isEmpty();
}

QVariantMap RegExpPair::serialize() const
{
    QVariantMap res;
    res["include"] = mInclude;
    res["exclude"] = mExclude;
    res["case"] = mCase;
    res["dotAll"] = mDotAll;
    res["multiline"] = mMultiline;
    return res;
}

void RegExpPair::deserealize(const QVariantMap &data)
{
    init(data.value("include").toString(),
         data.value("exclude").toString(),
         data.value("case", false).toBool(),
         data.value("dotAll", false).toBool(),
         data.value("multiline").toBool());
}

void RegExpPair::init(const QString& inc, const QString& exc, bool caseSensitive, bool dotAll, bool multiline) {
    mInclude = inc;
    mExclude = exc;

    QRegularExpression::PatternOptions opts;
    if (!caseSensitive) {
        opts = opts | QRegularExpression::CaseInsensitiveOption;
    }
    if (dotAll) {
        opts = opts | QRegularExpression::DotMatchesEverythingOption;
    }
    if (multiline) {
        opts = opts | QRegularExpression::MultilineOption;
    }

    mIncludeExp = QRegularExpression(inc, opts);
    mExcludeExp = QRegularExpression(exc, opts);
    mCase = caseSensitive;
    mDotAll = dotAll;
    mMultiline = multiline;
}

QString RegExpPair::include() const
{
    return mInclude;
}

QRegularExpression RegExpPair::includeExp() const {
    return mIncludeExp;
}

QRegularExpression RegExpPair::excludeExp() const
{
    return mExcludeExp;
}

QString RegExpPair::exclude() const
{
    return mExclude;
}

bool RegExpPair::case_() const
{
    return mCase;
}

bool RegExpPair::dotAll() const {
    return mDotAll;
}

bool RegExpPair::multiline() const {
    return mMultiline;
}

bool RegExpPair::match(const QString s, int* pos, int* len) const
{
    if (pos)
        *pos = 0;
    if (len)
        *len = 0;

    bool inc = false;
    if (mInclude.isEmpty()) {
        inc = true;
    } else {
        QRegularExpressionMatch m = mIncludeExp.match(s);
        if (m.hasMatch()) {
            int p = m.capturedStart();
            if (pos) {
                *pos = p;
            }
            if (len) {
                *len = m.capturedLength();
            }
            inc = true;
        } else {
            inc = false;
        }
    }
    return (inc) && (mExclude.isEmpty() || !mExcludeExp.match(s).hasMatch());
}



QStringList RegExpPair::exps() const {
    return {mInclude, mExclude};
}

void RegExpPair::test(const QStringList &paths, const RegExpPair &exp, const QList<bool> &matched)
{
    for (int i=0;i<paths.size();i++) {
        Q_ASSERT(exp.match(paths[i]) == matched[i]);
    }
}

// todo proper tests
void RegExpPair::test()
{

#if 0
    QStringList paths = {"foo.bar","foo.bar.baz","foo"};

    test(paths,RegExp("foo","",false),{true,true,true});
    test(paths,RegExp("bar","",false),{true,true,false});
    test(paths,RegExp("foo","baz",false),{true,false,true});
    test(paths,RegExp("","bar",false),{false,false,true});

    test(paths,RegExp("FOO","baz",true),{false,false,false});

    qDebug() << "passed RegExp::test()";

    int p;
    int l;
    RegExp e("b","",false);
    e.match(paths[0],&p,&l);
    Q_ASSERT(p == 4 && l == 1);
#endif
}

QDebug operator <<(QDebug &debug, const RegExpPair &exp)
{
    QString exp_ = QString("RegExp(\"%1\", \"%2\", %3, %4, %5)")
            .arg(exp.include())
            .arg(exp.exclude())
            .arg(exp.case_() ? "true" : "false")
            .arg(exp.dotAll() ? "true" : "false")
            .arg(exp.multiline() ? "true" : "false");
    debug.nospace() << exp_.toStdString().c_str();
    return debug;
}
