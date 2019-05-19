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
    static Settings* instance();

    void load();
    void save();

    QString editor(const QString& path) const;

    void toModel(QAbstractItemModel* model);
    void fromModel(QAbstractItemModel* model);

    QString settingsPath() const;

    QString error() const;

    QJsonArray sessions() const;

    void setSessions(const QJsonArray& value);

    void setExps(const QJsonObject &value);

    QJsonObject exps() const;
protected:
    static Settings* mInstance;

    QList<Editor> mEditors;
    QString mError;

    QJsonArray mSessions;
    QJsonObject mExps;
    QString mDir;

    Settings();
};

#endif // SETTINGS_H
