#include "rxcollector.h"

#include <QStringListModel>
#include <QDebug>
#include <QVariantList>
#include <QJsonObject>
#include <QJsonArray>
#include "utils.h"

#include "rxreplaceinput.h"
#include "rxpathinput.h"
#include "rxinput.h"


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
    mPathPatterns = prependModels(mPathPatterns, exp.exps());
}

void RXCollector::collect(const RegExp &exp)
{
    mPatterns = prependModels(mPatterns, exp.exps());
}

void RXCollector::collect(const QString& exp) {
    QStringList exps;
    exps << exp;
    mReplacements = prependModels(mReplacements, exps);
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
    for(int i=0;i<mPathPatterns.size();i++) {
        QComboBox* combo = input->input(i);
        QStringListModel* model = mPathPatterns[i];
        QString text = combo->currentText();
        combo->setModel(model);
        combo->setCurrentText(text);
        combo->setCompleter(nullptr);
    }
}

void RXCollector::load(RXInput *input) {
    for(int i=0;i<mPatterns.size();i++) {
        QComboBox* combo = input->input(i);
        QStringListModel* model = mPatterns[i];
        QString text = combo->currentText();
        combo->setModel(model);
        combo->setCurrentText(text);
        combo->setCompleter(nullptr);
    }
}

void RXCollector::load(RXReplaceInput *input) {
    for(int i=0;i<mReplacements.size();i++) {
        QComboBox* combo = input->input(i);
        QStringListModel* model = mReplacements[i];
        QString text = combo->currentText();
        combo->setModel(model);
        combo->setCurrentText(text);
        combo->setCompleter(nullptr);
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
    arrayOfArraysOfString(modelsLists(mPathPatterns), pathexps);
    QJsonArray exps;
    arrayOfArraysOfString(modelsLists(mPatterns),exps);
    QJsonArray replacements;
    arrayOfArraysOfString(modelsLists(mReplacements),replacements);
    j["pathexps"] = pathexps;
    j["pattern"] = exps;
    j["replacements"] = replacements;
}

QVariantList RXCollector::modelsLists(const QList<QStringListModel *> &models) {
    QVariantList exps;
    for(int i=0;i<models.size();i++) {
        QStringListModel* m = models[i];
        exps.append(m->stringList());
    }
    return exps;
}

void RXCollector::deserialize(const QList<QStringListModel *> &models, const QJsonArray& exps) {

    if (!exps.isEmpty()) {
        for(int i=0;i<models.size();i++) {
            if (i < exps.size()) {

                QJsonArray a = exps[i].toArray();
                QStringList vs = Utils::toStringList(a.toVariantList());
                QStringListModel* m = models[i];
                m->setStringList(vs);
            }
        }
    }
}

void RXCollector::deserialize(const QJsonObject &j)
{
    deserialize(mPathPatterns,j.value("pathexps").toArray());
    deserialize(mPatterns,j.value("pattern").toArray());
    deserialize(mReplacements,j.value("replacements").toArray());
}

QList<QStringListModel *> RXCollector::models()
{
    QList<QStringListModel *> result;
    result << mPathPatterns << mPatterns;
    return result;
}

RXCollector::RXCollector()
{
    for(int i=0;i<4;i++) {
        mPathPatterns << new QStringListModel();
    }
    for(int i=0;i<2;i++) {
        mPatterns << new QStringListModel();
    }
    mReplacements << new QStringListModel();
}
