#ifndef JSON_H
#define JSON_H

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#define JSON_HAS_STRING(json,k) (json.contains(k) && json[k].isString())
#define JSON_HAS_ARRAY(json,k) (json.contains(k) && json[k].isArray())

#endif // JSON_H
