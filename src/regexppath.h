#ifndef REGEXPPATH_H
#define REGEXPPATH_H

#include <QStringList>
#include <QMetaType>
#include <QRegularExpression>
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
    RegExpPath(const QStringList& regExps, bool case_, bool notBinary);
    RegExpPath(const QVariantMap& data);

    bool operator == (const RegExpPath& other) const;

    bool operator != (const RegExpPath& other) const;

    bool isEmpty() const;

    bool match(const QString& path) const;

    QStringList patterns() const;
    bool case_() const;

    bool notBinary() const;

    void setNotBinary(bool value);

    static void test();
    static void test(const QStringList &paths, const RegExpPath &exp, const QList<bool> &matched);

    QVariantMap serialize() const;
    void deserealize(const QVariantMap& data);
    void init(const QStringList& regExps, bool case_, bool notBinary);

    static QString getExt(const QString &path);
protected:
    QStringList mPatterns;
    QList<QRegularExpression> mPatterns_;
    bool mCase;
    bool mNotBinary;
};


QDebug operator <<(QDebug debug, const RegExpPath& path);

Q_DECLARE_METATYPE(RegExpPath)

#endif // REGEXPPATH_H
