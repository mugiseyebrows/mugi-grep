/*
http://mugiseyebrows.ru/web-utils/class.html
Editor
QString exts; QRegExp exp; QString app

*/
#ifndef EDITOR_H
#define EDITOR_H
#include <QString>
#include <QRegularExpression>
#include <QVariant>
class QJsonObject;

class Editor{
public:
/*********************** CONSTRUCTORS ***********************/
    Editor();
    Editor(const QJsonObject& json);
    Editor(const QString& exts, const QString& app);
/************************* GETTERS **************************/
    QString exts() const;
    QRegularExpression exp() const;
    QString app() const;
/************************* SETTERS **************************/
    void setExts(const QString& value);
    void setApp(const QString& value);
/************************ OPERATORS *************************/

/********************* MEMBER FUNCTIONS *********************/
    void fromJson(const QJsonObject& json);
    void toJson(QJsonObject &json);
protected:
    QString mExts;
    QRegularExpression mExp;
    QString mApp;
};
#endif //EDITOR_H
