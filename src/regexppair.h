#ifndef REGEXPPAIR_H
#define REGEXPPAIR_H

#include <QString>
#include <QMetaType>
#include <QRegularExpression>
#include <QVariantMap>

class RegExpPair {
public:
    RegExpPair();
    RegExpPair(const QString &inc, const QString &exc, bool caseSensitive = false, bool dotAll = false, bool multiline = false);
    /*RegExpPair(const QStringList& regExps, bool case_);*/
    RegExpPair(const QVariantMap& data);

    bool isEmpty() const;

    QString include() const;
    QString exclude() const;
    bool case_() const;

    bool match(const QString s, int* pos = 0, int* len = 0) const;

    static void test();
    static void test(const QStringList& paths, const RegExpPair& exp, const QList<bool>& matched);

    QVariantMap serialize() const;
    void deserealize(const QVariantMap& data);

    QStringList exps() const;
    QRegularExpression includeExp() const;

    QRegularExpression excludeExp() const;
    bool dotAll() const;
    bool multiline() const;
protected:

    void init(const QString &inc, const QString &exc, bool caseSensitive, bool dotAll, bool multiline);

    QString mInclude;
    QString mExclude;
    QRegularExpression mIncludeExp;
    QRegularExpression mExcludeExp;
    bool mCase;
    bool mDotAll;
    bool mMultiline;
};

QDebug operator <<(QDebug& debug, const RegExpPair& exp);

Q_DECLARE_METATYPE(RegExpPair)

#endif // REGEXPPAIR_H
