#ifndef RXCOLLECTOR_H
#define RXCOLLECTOR_H

#include "regexppath.h"
#include "regexp.h"

#include "widget/rxpathinput.h"
#include "widget/rxinput.h"

#include <QStringListModel>

class RXCollector
{
public:
    static RXCollector* instance();
    void collect(const RegExpPath& exp);
    void collect(const RegExp& exp);

    void load(RXPathInput* input);

    void serialize(QJsonObject& json);

    QList<QStringListModel *> prependModels(const QList<QStringListModel *> &models, const QStringList &exps);

    void load(RXInput *input);
    void deserialize(const QList<QStringListModel *> &models, const QJsonArray &exps);
    QVariantList modelsLists(const QList<QStringListModel *> &models);
    void deserialize(const QJsonObject &j);
    QList<QStringListModel*> models();

protected:
    RXCollector();
    static RXCollector* mInstance;

    QList<QStringListModel*> mPathExps;
    QList<QStringListModel*> mExps;
};

#endif // RXCOLLECTOR_H
