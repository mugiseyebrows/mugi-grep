// DO NOT EDIT ** This file was generated by mugicpp ** DO NOT EDIT
#ifndef REPLACEITEM_H
#define REPLACEITEM_H
#include <QMetaType>
#include <QString>

class ReplaceItem {

public:
    ReplaceItem();
    ReplaceItem(int line, const QString& before, const QString& after);
    int line() const;
    void setLine(int value);
    QString before() const;
    void setBefore(const QString& value);
    QString after() const;
    void setAfter(const QString& value);

protected:
    int mLine;
    QString mBefore;
    QString mAfter;
};

Q_DECLARE_METATYPE(ReplaceItem)

#endif // REPLACEITEM_H
