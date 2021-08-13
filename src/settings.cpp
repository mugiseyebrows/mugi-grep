#include "settings.h"

#include <QAbstractItemModel>
#include <QDir>

#include <QStandardPaths>
#include <QVariantList>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include "jsonhelper.h"
#include "regexppath.h"
#include "editorsmodel.h"

#define IS_DEBUG false

void Settings::load()
{

    if (IS_DEBUG) {
        return;
    }

    QString path = this->settingsPath();

    if (!QFile(path).exists()) {
        return;
    }

    QJsonDocument doc = loadJson(path);

    if (!doc.isObject()) {
        return;
    }

    QJsonObject settings = doc.object();

    mSessions = settings.value("sessions").toArray();
    mPatterns = settings.value("patterns").toObject();
    mPaths = settings.value("paths").toArray();
    mStyle = settings.value("style").toString();
    mViewOptions = settings.value("view").toObject();

    QJsonArray editors = settings.value("editors").toArray();

    for(QJsonValue editor: editors) {
        mEditors.append(Editor::fromJson(editor.toObject()));
    }
}

QString Settings::settingsPath() const
{
    return QDir(mDir).filePath("settings.json");
}

QString Settings::error() const
{
    return mError;
}

QJsonArray Settings::sessions() const
{
    return mSessions;
}

QJsonObject Settings::patterns() const {
    return mPatterns;
}

QJsonArray Settings::paths() const {
    return mPaths;
}

QJsonObject Settings::viewOptions() const {
    return mViewOptions;
}

QString Settings::style() const
{
    return mStyle;
}

void Settings::setStyle(const QString& style) {
    mStyle = style;
}

void Settings::setSessions(const QJsonArray &value)
{
    mSessions = value;
}

void Settings::setPatterns(const QJsonObject &value)
{
    mPatterns = value;
}

void Settings::setPaths(const QJsonArray &value) {
    mPaths = value;
}

void Settings::setViewOptions(const QJsonObject &value) {
    mViewOptions = value;
}

void Settings::save()
{
    if (IS_DEBUG) {
        return;
    }

    QJsonObject settings;

    QJsonArray editors;
    for(const Editor& editor: mEditors) {
        editors << editor.toJson();
    }
    settings["editors"] = editors;
    settings["sessions"] = mSessions;
    settings["patterns"] = mPatterns;
    settings["paths"] = mPaths;
    settings["style"] = mStyle;
    settings["view"] = mViewOptions;

    QString path = this->settingsPath();
    saveJson(path, settings);

    qDebug() << "saved to" << path;
}

void Settings::associate(const QString& path, const Editor& editor, bool permanent) {
    QFileInfo info(path);
    QString ext = info.suffix();
    if (permanent && !ext.isNull()) {
        QStringList group = EditorsModel::extGroup(ext);
        Editor editor_ = editor;
        editor_.setExts(group.join("|"));
        mEditors.append(editor_);
    } else {
        mTemporaryEditors[path] = editor;
    }
}

QString Settings::editor(const QString &path) const
{

    if (mEditors.isEmpty()) {
        return QString();
    }

    QString ext = RegExpPath::getExt(path);

    for(const Editor& editor: mEditors) {
        if (editor.matches(ext)) {
            return editor.app();
        }
    }

    if (mTemporaryEditors.contains(path)) {
        return mTemporaryEditors[path].app();
    }

    return QString();
}

Settings::Settings()
{
    QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QString name = qApp->applicationName();
    QDir d(appData);
    if (!d.exists()) {
        d.cdUp();
        if (!d.mkdir(name)) {
            QString error = QString("Can not create directory %1").arg(appData);
            QMessageBox::critical(qApp->activeWindow(),"Error",error);
        }
        d.cd(name);
    }
    mDir = appData;
    load();
}

void Settings::toModel(QAbstractItemModel *model)
{
    for(int i=0;i<model->rowCount();i++) {
        QString exts;
        QString app;
        if (mEditors.size() > i) {
            exts = mEditors[i].exts();
            app = mEditors[i].app();
        }
        model->setData(model->index(i,0),exts);
        model->setData(model->index(i,1),app);
    }
}

void Settings::fromModel(QAbstractItemModel *model)
{
    mEditors.clear();
    for(int i=0;i<model->rowCount();i++) {
        QString exts = model->data(model->index(i,0)).toString();
        QString app = model->data(model->index(i,1)).toString();
        if (!exts.isEmpty() && !app.isEmpty()) {
            mEditors.append(Editor(exts,app));
        }
    }
}
