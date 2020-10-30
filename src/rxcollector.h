#ifndef RXCOLLECTOR_H
#define RXCOLLECTOR_H

#include "regexppath.h"
#include "regexp.h"

class RXInput;
class RXPathInput;
class RXReplaceInput;
class QLineEdit;

#include <QStringListModel>

class RXCollector
{
public:
    static RXCollector* instance();
    void collect(const RegExpPath& exp);
    void collect(const RegExp& exp);
    void collectReplacement(const QString &exp);
    void collectPath(const QString &exp);

    void load(RXPathInput* input);
    void load(RXInput *input);
    void load(RXReplaceInput *input);

    void load(QLineEdit* edit);

    QJsonObject serializePatterns();

    QList<QStringListModel *> prependModels(const QList<QStringListModel *> &models, const QStringList &exps);

    void deserializePatterns(const QList<QStringListModel *> &models, const QJsonArray &exps);
    QVariantList modelsLists(const QList<QStringListModel *> &models);
    void deserializePatterns(const QJsonObject &j);
    QList<QStringListModel*> models();

    void deserializePatterns(QStringList &paths, const QJsonArray &arr);
    QJsonArray serializePaths();
    void deserializePaths(const QJsonArray &arr);
protected:
    RXCollector();
    static RXCollector* mInstance;

    QList<QStringListModel*> mPathPatterns;
    QList<QStringListModel*> mPatterns;
    QList<QStringListModel*> mReplacements;
    QStringList mPaths;
};

#endif // RXCOLLECTOR_H
