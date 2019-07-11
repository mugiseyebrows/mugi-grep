#ifndef RXCOLLECTOR_H
#define RXCOLLECTOR_H

#include "regexppath.h"
#include "regexp.h"

class RXInput;
class RXPathInput;
class RXReplaceInput;

#include <QStringListModel>

class RXCollector
{
public:
    static RXCollector* instance();
    void collect(const RegExpPath& exp);
    void collect(const RegExp& exp);
    void collect(const QString &exp);

    void load(RXPathInput* input);
    void load(RXInput *input);
    void load(RXReplaceInput *input);

    void serialize(QJsonObject& json);

    QList<QStringListModel *> prependModels(const QList<QStringListModel *> &models, const QStringList &exps);

    void deserialize(const QList<QStringListModel *> &models, const QJsonArray &exps);
    QVariantList modelsLists(const QList<QStringListModel *> &models);
    void deserialize(const QJsonObject &j);
    QList<QStringListModel*> models();



protected:
    RXCollector();
    static RXCollector* mInstance;

    QList<QStringListModel*> mPathExps;
    QList<QStringListModel*> mExps;
    QList<QStringListModel*> mReplacements;
};

#endif // RXCOLLECTOR_H
