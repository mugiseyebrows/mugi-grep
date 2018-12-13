#ifndef REGEXP_H
#define REGEXP_H

#include <QString>
#include <QMetaType>
#include <QRegularExpression>
#include <QVariantMap>

class RegExp {
public:
    RegExp();
    RegExp(const QString& inc, const QString& exc, bool case_);
    RegExp(const QStringList& regExps, bool case_);
    RegExp(const QVariantMap& data);

    QString include() const;
    QString exclude() const;
    bool case_() const;

    bool match(const QString s, int* pos = 0, int* len = 0) const;

    static void test();
    static void test(const QStringList& paths, const RegExp& exp, const QList<bool>& matched);

    QVariantMap serialize() const;
    void deserealize(const QVariantMap& data);

    QStringList exps() const;
protected:

    void init(const QString &inc, const QString &exc, bool case_);

    QString mInclude;
    QString mExclude;
    QRegularExpression mInclude_;
    QRegularExpression mExclude_;
    bool mCase;
};

Q_DECLARE_METATYPE(RegExp)

#endif // REGEXP_H
