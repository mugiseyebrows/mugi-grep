#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

bool saveJson(const QString& path, const QJsonArray& data);
bool saveJson(const QString& path, const QJsonObject& data);

QJsonDocument loadJson(const QString& path, bool* ok = 0);

#endif // JSONHELPER_H
