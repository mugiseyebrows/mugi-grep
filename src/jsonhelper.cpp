#include "jsonhelper.h"

#include <QJsonDocument>
#include <QDebug>
#include <QFile>

bool saveJson(const QString& path, const QJsonObject& data) {
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly)) {
        qDebug() << ("cant open file " + path);
        return false;
    }
    QJsonDocument doc(data);
    f.write(doc.toJson());
    f.close();
    return true;
}

bool saveJson(const QString& path, const QJsonArray& data) {
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly)) {
        qDebug() << ("cant open file " + path);
        return false;
    }
    QJsonDocument doc(data);
    f.write(doc.toJson());
    f.close();
    return true;
}

QJsonDocument loadJson(const QString& path, bool* ok) {
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        if (ok) {
            *ok = false;
        }
        return QJsonDocument();
    }
    QByteArray data = f.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    if (ok) {
        *ok = true;
    }
    return doc;
}

