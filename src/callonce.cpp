#include "callonce.h"

#include <QTimer>

CallOnce::CallOnce(const QString name, int timeout, QObject *parent) : QObject(parent), mName(name), mId(0), mTimeout(timeout)
{

}

void CallOnce::setName(const QString &name)
{
    mName = name;
}

void CallOnce::setTimeout(int timeout)
{
    mTimeout = timeout;
}

QString CallOnce::name() const
{
    return mName;
}

int CallOnce::timeout() const
{
    return mTimeout;
}

void CallOnce::onPost()
{
    mId++;
    int id = mId;
    QTimer::singleShot(mTimeout,[=](){
        if (id == mId) {
            //qDebug() << mName << "id == mId" << id << mId;
            emit call();
        } else {
            //qDebug() << mName << "id != mId" << id << mId;
        }
    });
}

