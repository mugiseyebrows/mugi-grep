#include "regexppath.h"

#include "lit.h"
#include <QDebug>

RegExpPath::RegExpPath()
{
    init(QStringList(),false,true);
}

void RegExpPath::init(const QStringList &regExps, bool case_, bool notBinary)
{
    QStringList regExps_ = regExps;
    while(regExps_.size() < 4) {
        regExps_ << QString();
    }
    mPatterns = regExps_;
    mCase = case_;
    mNotBinary = notBinary;
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
    init(data.value("pattern").toStringList(),data.value("case", false).toBool(), data.value("notBinary", true).toBool());
}

RegExpPath::RegExpPath(const QStringList& regExps, bool case_, bool notBinary)
{
    init(regExps,case_,notBinary);
}

RegExpPath::RegExpPath(const QVariantMap &data)
{
    deserealize(data);
}

bool RegExpPath::operator ==(const RegExpPath &other) const
{
    return other.patterns() == patterns() && other.case_() == case_() && other.notBinary() == notBinary();
}

bool RegExpPath::operator !=(const RegExpPath &other) const
{
    return !(*this == other);
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
    res["notBinary"] = mNotBinary;
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
    int q = qMax(path.lastIndexOf("/"), path.lastIndexOf("\\"));
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


QStringList RegExpPath::patterns() const
{
    return mPatterns;
}

bool RegExpPath::case_() const
{
    return mCase;
}

bool RegExpPath::notBinary() const
{
    return mNotBinary;
}

void RegExpPath::setNotBinary(bool value)
{
    mNotBinary = value;
}

void RegExpPath::test(const QStringList& paths, const RegExpPath& exp, const QList<bool>& matched) {
    for (int i=0;i<paths.size();i++) {
        Q_ASSERT(exp.match(paths[i]) == matched[i]);
    }
}

void RegExpPath::test()
{
    QStringList paths = {"foo.bar", "foo.bar.baz", "foo"};
    test(paths,RegExpPath({"","bar","",""},false,true),{true,false,false});
    test(paths,RegExpPath({"","foo","",""},false,true),{false,false,false});
    test(paths,RegExpPath({"bar","","",""},false,true),{true,true,false});
    test(paths,RegExpPath({"bar","","","baz"},false,true),{true,false,false});
    test(paths,RegExpPath({"foo","","",""},false,true),{true,true,true});
    test(paths,RegExpPath({"foo","","","baz"},false,true),{true,false,true});
    test(paths,RegExpPath({"foo","","bar","baz"},false,true),{false,false,true});
    test(paths,RegExpPath({"foo","bar","","baz"},false,true),{true,false,false});
    test(paths,RegExpPath({"FOO","","",""},true,true),{false,false,false});
    qDebug() << "passed RegExpPath::test()";
}

QDebug operator <<(QDebug debug, const RegExpPath &path)
{
    debug.space() << "RegExpPath(" << path.patterns() << path.case_() << path.notBinary() << ")";
    return debug.space();
}
