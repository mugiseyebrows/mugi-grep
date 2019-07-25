#include "regexp.h"

#include "lit.h"

#include <QStringList>
#include <QDebug>

RegExp::RegExp()
{
    init(QString(),QString(),false);
}

RegExp::RegExp(const QString &inc, const QString &exc, bool case_)
{
    init(inc,exc,case_);
}

RegExp::RegExp(const QStringList &regExps, bool case_)
{
    init(regExps[0],regExps[1],case_);
}

RegExp::RegExp(const QVariantMap &data)
{
    deserealize(data);
}

bool RegExp::isEmpty() const
{
    return mInclude.isEmpty() && mExclude.isEmpty();
}

QVariantMap RegExp::serialize() const
{
    QVariantMap res;
    res["include"] = mInclude;
    res["exclude"] = mExclude;
    res["case"] = mCase;
    return res;
}

void RegExp::deserealize(const QVariantMap &data)
{
    init(data.value("include").toString(),data.value("exclude").toString(),data.value("case").toBool());
}

void RegExp::init(const QString& inc, const QString& exc, bool case_) {
    mInclude = inc;
    mExclude = exc;
    mIncludeExp = QRegularExpression(inc, case_ ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption);
    mExcludeExp = QRegularExpression(exc, case_ ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption);
    mCase = case_;
}

QString RegExp::include() const
{
    return mInclude;
}

QRegularExpression RegExp::includeExp() const {
    return mIncludeExp;
}

QString RegExp::exclude() const
{
    return mExclude;
}

bool RegExp::case_() const
{
    return mCase;
}

bool RegExp::match(const QString s, int* pos, int* len) const
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



QStringList RegExp::exps() const {
    return Lit::sl(mInclude,mExclude);
}

void RegExp::test(const QStringList &paths, const RegExp &exp, const QList<bool> &matched)
{
    for (int i=0;i<paths.size();i++) {
        Q_ASSERT(exp.match(paths[i]) == matched[i]);
    }
}

void RegExp::test()
{

    QStringList paths = {"foo.bar","foo.bar.baz","foo"};

    test(paths,RegExp("foo","",false),Lit::bl(true,true,true));
    test(paths,RegExp("bar","",false),Lit::bl(true,true,false));
    test(paths,RegExp("foo","baz",false),Lit::bl(true,false,true));
    test(paths,RegExp("","bar",false),Lit::bl(false,false,true));

    test(paths,RegExp("FOO","baz",true),Lit::bl(false,false,false));

    qDebug() << "passed RegExp::test()";

    int p;
    int l;
    RegExp e("b","",false);
    e.match(paths[0],&p,&l);
    Q_ASSERT(p == 4 && l == 1);
}

QDebug operator <<(QDebug &debug, const RegExp &exp)
{
    QString exp_ = QString("RegExp(\"%1\", \"%2\", %3)").arg(exp.include()).arg(exp.exclude()).arg(exp.case_() ? "true" : "false");
    debug.nospace() << exp_.toStdString().c_str();
    return debug;
}
