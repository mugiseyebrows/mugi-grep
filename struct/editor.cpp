#include "struct/editor.h"
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
QRegExp Editor::exp() const {
    return mExp;
}
QString Editor::app() const {
    return mApp;
}
/************************* SETTERS **************************/
void Editor::setExts(const QString& value) {
    mExts = value;
    mExp = QRegExp("^(" + mExts + ")$");
}

void Editor::setApp(const QString& value) {
    mApp = value;
}

void Editor::fromJson(const QJsonObject &json)
{
    if (JSON_HAS_STRING(json,"exts")) {
        setExts(json["exts"].toString());
    }
    if (JSON_HAS_STRING(json,"app")) {
        setApp(json["app"].toString());
    }
}

void Editor::toJson(QJsonObject &json)
{
    json["exts"] = mExts;
    json["app"] = mApp;
}

/********************* MEMBER FUNCTIONS *********************/
