#include "regexppath.h"

#include "utils/sl.h"
#include "utils/bl.h"
#include <QDebug>

RegExpPath::RegExpPath()
{
}



void RegExpPath::init(const QStringList &regExps, bool case_)
{
    mRegExps = regExps;
    mCase = case_;
    for (int i=0;i<regExps.size();i++) {
        Qt::CaseSensitivity case__ = case_ ? Qt::CaseSensitive : Qt::CaseInsensitive;
        if (i % 2 == 1) {
            mRegExps_ << QRegExp("^(" + regExps[i] + ")$", case__);
        } else {
            mRegExps_ << QRegExp(regExps[i], case__);
        }
    }
}

void RegExpPath::deserealize(const QVariantMap &data)
{
    init(data.value("exps").toStringList(),data.value("case").toBool());
}

RegExpPath::RegExpPath(const QStringList& regExps, bool case_)
{
    init(regExps,case_);
}

RegExpPath::RegExpPath(const QVariantMap &data)
{
    deserealize(data);
}

QVariantMap RegExpPath::serialize() const
{
    QVariantMap res;
    res["exps"] = mRegExps;
    res["case"] = mCase;
    return res;
}

bool RegExpPath::match(const QString &path) const
{
    QString ext;
    int p = path.lastIndexOf(".");
    if (p > -1)
        ext = path.mid(p+1);
    return (mRegExps[PathInclude].isEmpty() || mRegExps_[PathInclude].indexIn(path) > -1) &&
            (mRegExps[ExtInclude].isEmpty() || mRegExps_[ExtInclude].indexIn(ext) > -1) &&
            (mRegExps[PathExclude].isEmpty() || mRegExps_[PathExclude].indexIn(path) < 0) &&
            (mRegExps[ExtExclude].isEmpty() || mRegExps_[ExtExclude].indexIn(ext) < 0);
}

QStringList RegExpPath::exps() const
{
    return mRegExps;
}

bool RegExpPath::case_() const
{
    return mCase;
}

void RegExpPath::test(const QStringList& paths, const RegExpPath& exp, const QList<bool>& matched) {
    for (int i=0;i<paths.size();i++) {
        Q_ASSERT(exp.match(paths[i]) == matched[i]);
    }
}

void RegExpPath::test()
{

    QStringList paths;
    paths << "foo.bar" << "foo.bar.baz" << "foo";

    test(paths,RegExpPath(sl("","bar","",""),false),bl(true,false,false));
    test(paths,RegExpPath(sl("","foo","",""),false),bl(false,false,false));
    test(paths,RegExpPath(sl("bar","","",""),false),bl(true,true,false));
    test(paths,RegExpPath(sl("bar","","","baz"),false),bl(true,false,false));
    test(paths,RegExpPath(sl("foo","","",""),false),bl(true,true,true));
    test(paths,RegExpPath(sl("foo","","","baz"),false),bl(true,false,true));
    test(paths,RegExpPath(sl("foo","","bar","baz"),false),bl(false,false,true));
    test(paths,RegExpPath(sl("foo","bar","","baz"),false),bl(true,false,false));

    test(paths,RegExpPath(sl("FOO","","",""),true),bl(false,false,false));

    qDebug() << "passed RegExpPath::test()";
}



