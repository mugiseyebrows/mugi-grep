#ifndef SEARCHBROWSER_H
#define SEARCHBROWSER_H
#include "regexp.h"
#include "regexppath.h"
#include "searchparams.h"
#include <QDate>
#include <QDateTime>
#include <QString>
#include <QTextBrowser>
#include <QWidget>

class QDebug;
class SearchBrowser : public QTextBrowser {
    Q_OBJECT
public:
    SearchBrowser(QWidget* parent = 0);
    ~SearchBrowser();
    int searchId() const;
    void setSearchId(int searchId);
    RegExp exp() const;
    void setExp(const RegExp& exp);
    RegExpPath filter() const;
    void setFilter(const RegExpPath& filter);
    int linesBefore() const;
    void setLinesBefore(int linesBefore);
    int linesAfter() const;
    void setLinesAfter(int linesAfter);
    bool showFileName() const;
    void setShowFileName(bool showFileName);
    bool showLineNumber() const;
    void setShowLineNumber(bool showLineNumber);
    bool onlyMatched() const;
    void setOnlyMatched(bool onlyMatched);
    bool notBinary() const;
    void setNotBinary(bool notBinary);
    QDateTime changed() const;
    void setChanged(const QDateTime& changed);
    QString replacement() const;
    void setReplacement(const QString& replacement);
    bool preserveCase() const;
    void setPreserveCase(bool preserveCase);
    bool isExecuted() const;
    void copy(SearchBrowser* dest);
    SearchParams params(int action, int id, const QString& path, bool cacheFileList);

protected:
    int mSearchId;
    RegExp mExp;
    RegExpPath mFilter;
    int mLinesBefore;
    int mLinesAfter;
    bool mShowFileName;
    bool mShowLineNumber;
    bool mOnlyMatched;
    bool mNotBinary;
    QDateTime mChanged;
    QString mReplacement;
    bool mPreserveCase;
};
#endif // SEARCHBROWSER_H
