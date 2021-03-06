// DO NOT EDIT ** This file was generated by mugicpp ** DO NOT EDIT
#include "editor.h"
#include <QMap>

Editor::Editor() : mType(Type::NotSpecified) {
}
Editor::Editor(const QString& exts, const QString& app, Editor::Type type)
    : mExts(exts), mApp(app), mType(type) {
    setExts(exts);
}
QString Editor::exts() const {
    return mExts;
}
QString Editor::app() const {
    return mApp;
}
void Editor::setApp(const QString& value) {
    mApp = value;
}
Editor::Type Editor::type() const {
    return mType;
}
void Editor::setType(Editor::Type value) {
    mType = value;
}
QRegularExpression Editor::exp() const {
    return mExp;
}
void Editor::setExp(const QRegularExpression& value) {
    mExp = value;
}
QString Editor::name() const {
    static QMap<Editor::Type, QString> names = {
        {Type::VsCode, "VsCode"},
        {Type::QtCreator, "QtCreator"},
        {Type::Geany, "Geany"},
    };
    return names.value(mType);
}
void Editor::setExts(const QString& value) {

    mExts = value;
    mExp = QRegularExpression("^(" + mExts + ")$");
}
bool Editor::isNull() const {
    return mApp.isEmpty();
}
bool Editor::matches(const QString& ext) const {
    return mExp.match(ext).hasMatch();
}
Editor Editor::fromJson(const QJsonObject& json) {
    return Editor(json.value("exts").toString(), json.value("app").toString());
}
QJsonObject Editor::toJson() const {
    QJsonObject json;
    json["exts"] = mExts;
    json["app"] = mApp;
    return json;
}
