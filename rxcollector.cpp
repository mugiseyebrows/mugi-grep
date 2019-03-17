#include "rxcollector.h"

#include <QStringListModel>
#include <QDebug>
#include <QVariantList>
#include <QJsonObject>
#include <QJsonArray>

RXCollector *RXCollector::mInstance = nullptr;

#define COLLECTION_SIZE 20

RXCollector *RXCollector::instance()
{
    if (!mInstance) {
        mInstance = new RXCollector();
    }
    return mInstance;
}

namespace {

QObjectList toObjectList(const QList<QStringListModel*>& models) {
    QObjectList result;
    foreach(QStringListModel* model, models) {
        result << model;
    }
    return result;
}

}

void RXCollector::collect(const RegExpPath &exp)
{
    mTrash.append(toObjectList(mPathExps));
    mPathExps = prependModels(mPathExps, exp.exps());
    //qDebug() << mTrash.size() << "models in trash";
}

void RXCollector::collect(const RegExp &exp)
{
    mTrash.append(toObjectList(mExps));
    mExps = prependModels(mExps, exp.exps());
    //qDebug() << mTrash.size() << "models in trash";
}

QList<QStringListModel *> RXCollector::prependModels(const QList<QStringListModel *> &models, const QStringList &exps) {

    //qDebug() << "prependModels" << models.size();
    QList<QStringListModel *> models_;
    for(int i=0;i<models.size();i++) {
        QStringListModel* model = qobject_cast<QStringListModel*>(models[i]);
        QString s = exps.value(i);
        QStringList items = model->stringList();
        QStringList items_;
        items_.append(s);
        std::copy_if(items.begin(),items.end(),std::back_inserter(items_),
                     [&](const QString& item){return item != s;});
        if (items_.size() > COLLECTION_SIZE) {
            items_.removeLast();
        }
        //qDebug() << s << items << items_;
        QStringListModel *model_ = new QStringListModel();
        model_->setStringList(items_);
        models_ << model_;
    }
    return models_;
}

void RXCollector::load(RXPathInput *input)
{
    for(int i=0;i<mPathExps.size();i++) {
        QComboBox* combo = input->input(i);
        QStringListModel* model = mPathExps[i];
        QString text = combo->currentText();
        combo->setModel(model);
        combo->setCurrentText(text);
    }
}

void RXCollector::load(RXInput *input) {
    for(int i=0;i<mExps.size();i++) {
        QComboBox* combo = input->input(i);
        QStringListModel* model = mExps[i];
        QString text = combo->currentText();
        combo->setModel(model);
        combo->setCurrentText(text);
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
    deserialize(mPathExps,j.value("pathexps").toArray());
    deserialize(mExps,j.value("exps").toArray());
}

void RXCollector::clean()
{
    //qDebug() << "clean" << mTrash.size() << "models in trash";
    foreach (QObject* object, mTrash) {
        object->deleteLater();
    }
    mTrash.clear();
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
