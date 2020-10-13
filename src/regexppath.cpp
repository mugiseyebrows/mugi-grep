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
    mPatterns = regExps_;
    mCase = case_;
    for (int i=0;i<mPatterns.size();i++) {
        QRegularExpression::PatternOption opt = mCase ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption;
        if (i % 2 == 1) {
            mPatterns_ << QRegularExpression("^(" + mPatterns[i] + ")$", opt);
        } else {
            mPatterns_ << QRegularExpression(mPatterns[i], opt);
        }
    }
}

void RegExpPath::deserealize(const QVariantMap &data)
{
    init(data.value("pattern").toStringList(),data.value("case").toBool());
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
    return mPatterns[PathInclude].isEmpty() &&
            mPatterns[ExtInclude].isEmpty() &&
            mPatterns[PathExclude].isEmpty() &&
            mPatterns[ExtExclude].isEmpty();
}

QVariantMap RegExpPath::serialize() const
{
    QVariantMap res;
    res["pattern"] = mPatterns;
    res["case"] = mCase;
    return res;
}

bool RegExpPath::match(const QString &path) const
{
    QString ext = getExt(path);

    return (mPatterns[PathInclude].isEmpty() || mPatterns_[PathInclude].match(path).hasMatch()) &&
            (mPatterns[ExtInclude].isEmpty() || mPatterns_[ExtInclude].match(ext).hasMatch()) &&
            (mPatterns[PathExclude].isEmpty() || !mPatterns_[PathExclude].match(path).hasMatch()) &&
            (mPatterns[ExtExclude].isEmpty() || !mPatterns_[ExtExclude].match(ext).hasMatch());
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
    return mPatterns;
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
