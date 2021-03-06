#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

#include "editor.h"
#include <QList>
#include <QJsonArray>
#include <QJsonObject>

class QAbstractItemModel;

class Settings
{
public:
    Settings();

    void load();
    void save();

    QString editor(const QString& path) const;

    void toModel(QAbstractItemModel* model);
    void fromModel(QAbstractItemModel* model);

    QString settingsPath() const;

    QString error() const;

    QJsonArray sessions() const;

    void setSessions(const QJsonArray& value);

    void setPatterns(const QJsonObject &value);

    QJsonObject patterns() const;
    void setPaths(const QJsonArray &value);
    QJsonArray paths() const;

    QString style() const;

    void setStyle(const QString &style);
protected:

    QList<Editor> mEditors;
    QString mError;

    QJsonArray mSessions;
    QJsonObject mPatterns;
    QJsonArray mPaths;
    QString mDir;
    QString mStyle;

};

#endif // SETTINGS_H
