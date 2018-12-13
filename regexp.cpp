#include "regexp.h"

#include "utils/bl.h"
#include "utils/sl.h"

#include <QStringList>
#include <QDebug>

RegExp::RegExp()
{
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
    mInclude_ = QRegularExpression(inc, case_ ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption);
    mExclude_ = QRegularExpression(exc, case_ ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption);
}

QString RegExp::include() const
{
    return mInclude;
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
        QRegularExpression e = mInclude_;

        QRegularExpressionMatch m = e.match(s);

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
    return (inc) && (mExclude.isEmpty() || !mExclude_.match(s).hasMatch());
}



QStringList RegExp::exps() const {
    return sl(mInclude,mExclude);
}

void RegExp::test(const QStringList &paths, const RegExp &exp, const QList<bool> &matched)
{
    for (int i=0;i<paths.size();i++) {
        Q_ASSERT(exp.match(paths[i]) == matched[i]);
    }
}

void RegExp::test()
{

    QStringList paths;
    paths << "foo.bar" << "foo.bar.baz" << "foo";

    test(paths,RegExp("foo","",false),bl(true,true,true));
    test(paths,RegExp("bar","",false),bl(true,true,false));
    test(paths,RegExp("foo","baz",false),bl(true,false,true));
    test(paths,RegExp("","bar",false),bl(false,false,true));

    test(paths,RegExp("FOO","baz",true),bl(false,false,false));

    qDebug() << "passed RegExp::test()";

    int p;
    int l;
    RegExp e("b","",false);
    e.match(paths[0],&p,&l);
    Q_ASSERT(p == 4 && l == 1);


}
