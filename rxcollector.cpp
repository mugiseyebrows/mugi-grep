#include "rxcollector.h"

#include <QStringListModel>
#include <QDebug>
#include <QVariantList>

#include "utils/json.h"

RXCollector *RXCollector::mInstance = nullptr;

#define COLLECTION_SIZE 20

RXCollector *RXCollector::instance()
{
    if (!mInstance) {
        mInstance = new RXCollector();
    }
    return mInstance;
}

void RXCollector::collect(const RegExpPath &exp)
{
    prependModels(mPathExps, exp.exps());
}

void RXCollector::collect(const RegExp &exp)
{
    prependModels(mExps, exp.exps());
}

void RXCollector::prependModels(const QList<QStringListModel *> &models, const QStringList &exps) {

    qDebug() << "RXCollector::prependModels";
    for(int i=0;i<models.size();i++) {
        QStringListModel* m = qobject_cast<QStringListModel*>(models[i]);
        QString s = exps.value(i);

        QStringList items = m->stringList();
        int index = items.indexOf(s);
        if (index > -1) {
            items.removeAt(index);
        }

        items.prepend(s);
        m->setStringList(items);
        if (m->rowCount() > COLLECTION_SIZE) {
            m->removeRow(m->rowCount()-1);
        }
    }
}

void RXCollector::load(RXPathInput *input)
{
    for(int i=0;i<mPathExps.size();i++) {
        input->input(i)->setModel(mPathExps[i]);
        input->input(i)->setCurrentIndex(0);
    }
}

void RXCollector::load(RXInput *input) {
    for(int i=0;i<mExps.size();i++) {
        input->input(i)->setModel(mExps[i]);
        input->input(i)->setCurrentIndex(0);
    }
}


void arrayOfArraysOfString(const QVariantList& src, QJsonArray& dst) {
    for(QVariant v: src) {
        dst.append(QJsonArray::fromStringList(v.toStringList()));
    }
}

void RXCollector::serialize(QJsonObject &j)
{
    QJsonArray pathexps;
    arrayOfArraysOfString(modelsLists(mPathExps), pathexps);
    QJsonArray exps;
    arrayOfArraysOfString(modelsLists(mExps),exps);
    j["pathexps"] = pathexps;
    j["exps"] = exps;
}

QVariantList RXCollector::modelsLists(const QList<QStringListModel *> &models) {
    QVariantList exps;
    for(int i=0;i<models.size();i++) {
        QStringListModel* m = models[i];
        exps.append(m->stringList());
    }
    return exps;
}



QStringList toStringList(const QVariantList& vs) {
    QStringList s;
    std::transform(vs.begin(),vs.end(),std::back_inserter(s),
                   [&](QVariant v){return v.toString();});
    return s;
}


void RXCollector::deserialize(const QList<QStringListModel *> &models, const QJsonArray& exps) {

    if (!exps.isEmpty()) {
        for(int i=0;i<models.size();i++) {
            if (i < exps.size()) {

                QJsonArray a = exps[i].toArray();
                QStringList vs = toStringList(a.toVariantList());
                QStringListModel* m = models[i];
                m->setStringList(vs);
            }
        }
    }
}

void RXCollector::deserialize(const QJsonObject &j)
{
    if (JSON_HAS_ARRAY(j,"pathexps")) {
        deserialize(mPathExps,j["pathexps"].toArray());
    }
    if (JSON_HAS_ARRAY(j,"exps")) {
        deserialize(mExps,j["exps"].toArray());
    }
}


RXCollector::RXCollector()
{
    for(int i=0;i<4;i++) {
        mPathExps << new QStringListModel();
    }
    for(int i=0;i<2;i++) {
        mExps << new QStringListModel();
    }

}
