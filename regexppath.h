#ifndef REGEXPPATH_H
#define REGEXPPATH_H

#include <QStringList>
#include <QMetaType>
#include <QRegExp>
#include <QVariantMap>

class RegExpPath {
public:
    enum Index {
        PathInclude,
        ExtInclude,
        PathExclude,
        ExtExclude
    };

    RegExpPath();
    RegExpPath(const QStringList& regExps, bool case_);
    RegExpPath(const QVariantMap& data);
    bool match(const QString& path) const;

    QStringList exps() const;
    bool case_() const;

    static void test();
    static void test(const QStringList &paths, const RegExpPath &exp, const QList<bool> &matched);

    QVariantMap serialize() const;
    void deserealize(const QVariantMap& data);
    void init(const QStringList& regExps, bool case_);

protected:
    QStringList mRegExps;
    QList<QRegExp> mRegExps_;
    bool mCase;
};

Q_DECLARE_METATYPE(RegExpPath)

#endif // REGEXPPATH_H
