#ifndef SEARCHPARAMS_H
#define SEARCHPARAMS_H
#include "regexppair.h"
#include "regexppath.h"
#include "regexpreplacement.h"
#include <QMetaType>
#include <QString>

class SearchParams {

public:
    SearchParams();
    int id() const;
    void setId(int value);
    QString path() const;
    void setPath(const QString& value);
    RegExpPair pattern() const;
    void setPattern(const RegExpPair& value);
    RegExpPath filter() const;
    void setFilter(const RegExpPath& value);
    RegExpReplacement replacement() const;
    void setReplacement(const RegExpReplacement& value);
    bool cacheFileList() const;
    void setCacheFileList(bool value);

protected:
    int mId;
    QString mPath;
    RegExpPair mPattern;
    RegExpPath mFilter;
    RegExpReplacement mReplacement;
    bool mCacheFileList;
};

Q_DECLARE_METATYPE(SearchParams)

#endif // SEARCHPARAMS_H
