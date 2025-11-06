#include "regexppath.h"

#include <QDebug>
#include <QFileInfo>

RegExpPath::RegExpPath()
{
    init(QStringList(), false, false);
}

void RegExpPath::init(const QStringList &patterns, bool case_, bool binary)
{
    QStringList patterns1 = patterns;
    while(patterns1.size() < 4) {
        patterns1 << QString();
    }
    mPatterns = patterns1;
    mCase = case_;
    mBinary = binary;
    for (int i=0;i<mPatterns.size();i++) {
        QRegularExpression::PatternOption opt = mCase ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption;
        if (i % 2) {
            mExprs.append(QRegularExpression("^(" + mPatterns[i] + ")$", opt));
        } else {
            mExprs.append(QRegularExpression(mPatterns[i], opt));
        }
    }

}

void RegExpPath::deserealize(const QVariantMap &data)
{
    init(data.value("pattern").toStringList(),
         data.value("case", false).toBool(),
         data.value("binary", false).toBool());
}

RegExpPath::RegExpPath(const QStringList& regExps, bool case_, bool binary)
{
    init(regExps,case_,binary);
}

RegExpPath::RegExpPath(const QVariantMap &data)
{
    deserealize(data);
}

bool RegExpPath::operator ==(const RegExpPath &other) const
{
    return other.patterns() == patterns() && other.case_() == case_() && other.binary() == binary();
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
    res["binary"] = mBinary;
    return res;
}

bool RegExpPath::match(const QString &path) const
{
    QString ext = getExt(path);

    return (mPatterns[PathInclude].isEmpty() || mExprs[PathInclude].match(path).hasMatch()) &&
            (mPatterns[ExtInclude].isEmpty() || mExprs[ExtInclude].match(ext).hasMatch()) &&
            (mPatterns[PathExclude].isEmpty() || !mExprs[PathExclude].match(path).hasMatch()) &&
            (mPatterns[ExtExclude].isEmpty() || !mExprs[ExtExclude].match(ext).hasMatch());
}



QString RegExpPath::getExt(const QString& path) {
#if 0
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
#endif
    return QFileInfo(path).suffix();
}


QStringList RegExpPath::patterns() const
{
    return mPatterns;
}

bool RegExpPath::case_() const
{
    return mCase;
}

bool RegExpPath::binary() const
{
    return mBinary;
}

void RegExpPath::setBinary(bool value)
{
    mBinary = value;
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
    debug.space() << "RegExpPath(" << path.patterns() << path.case_() << path.binary() << ")";
    return debug.space();
}
