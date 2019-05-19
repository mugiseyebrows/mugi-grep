#include "regexppath.h"

#include "lit.h"
#include <QDebug>

RegExpPath::RegExpPath()
{
    init(QStringList(),false);
}

void RegExpPath::init(const QStringList &regExps, bool case_)
{
    QStringList regExps_ = regExps;
    while(regExps_.size() < 4) {
        regExps_ << QString();
    }
    mRegExps = regExps_;
    mCase = case_;
    for (int i=0;i<mRegExps.size();i++) {
        QRegularExpression::PatternOption opt = mCase ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption;
        if (i % 2 == 1) {
            mRegExps_ << QRegularExpression("^(" + mRegExps[i] + ")$", opt);
        } else {
            mRegExps_ << QRegularExpression(mRegExps[i], opt);
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

bool RegExpPath::isEmpty() const
{
    return mRegExps[PathInclude].isEmpty() &&
            mRegExps[ExtInclude].isEmpty() &&
            mRegExps[PathExclude].isEmpty() &&
            mRegExps[ExtExclude].isEmpty();
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
    QString ext = getExt(path);

    return (mRegExps[PathInclude].isEmpty() || mRegExps_[PathInclude].match(path).hasMatch()) &&
            (mRegExps[ExtInclude].isEmpty() || mRegExps_[ExtInclude].match(ext).hasMatch()) &&
            (mRegExps[PathExclude].isEmpty() || !mRegExps_[PathExclude].match(path).hasMatch()) &&
            (mRegExps[ExtExclude].isEmpty() || !mRegExps_[ExtExclude].match(ext).hasMatch());
}

QString RegExpPath::getExt(const QString& path) {
    int p = path.lastIndexOf(".");
    int q = path.lastIndexOf("/");
    if (p > -1) {
        if (q > -1) {
            if (q > p) {
                return QString();
            }
        }
        return path.mid(p+1).toLower();
    }
    return QString();
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
    using namespace Lit;

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


QDebug operator <<(QDebug debug, const RegExpPath &path)
{
    debug.space() << "RegExpPath(" << path.exps() << path.case_() << ")";
    return debug.space();
}
