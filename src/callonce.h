#ifndef CALLONCE_H
#define CALLONCE_H

#include <QObject>
#include <QDebug>

class CallOnce : public QObject
{
    Q_OBJECT
public:
    explicit CallOnce(const QString name = QString(), int timeout = 0, QObject *parent = nullptr);

    void setName(const QString& name);
    void setTimeout(int timeout);
    QString name() const;
    int timeout() const;

protected:
    int mId;
    QString mName;
    int mTimeout;
signals:
    void call();
public slots:
    void onPost();
};

#endif // CALLONCE_H
