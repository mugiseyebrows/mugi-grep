#include "editor.h"
#include <QJsonObject>

/*********************** CONSTRUCTORS ***********************/
Editor::Editor() {
}

Editor::Editor(const QJsonObject &json)
{
    fromJson(json);
}
Editor::Editor(const QString& exts, const QString& app) {
    setExts(exts);
    setApp(app);
}
/************************* GETTERS **************************/
QString Editor::exts() const {
    return mExts;
}
QRegularExpression Editor::exp() const {
    return mExp;
}
QString Editor::app() const {
    return mApp;
}
/************************* SETTERS **************************/
void Editor::setExts(const QString& value) {
    mExts = value;
    mExp = QRegularExpression("^(" + mExts + ")$");
}

void Editor::setApp(const QString& value) {
    mApp = value;
}

void Editor::fromJson(const QJsonObject &json)
{
    setExts(json.value("exts").toString());
    setApp(json.value("app").toString());
}

void Editor::toJson(QJsonObject &json)
{
    json["exts"] = mExts;
    json["app"] = mApp;
}

/********************* MEMBER FUNCTIONS *********************/