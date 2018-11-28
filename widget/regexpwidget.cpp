#include "widget/regexpwidget.h"
#include "regexp.h"


#include <QHBoxLayout>

#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>

#include <QStringList>
#include <QStringListModel>
#include <QToolButton>
#include <QVariantMap>

RegExpWidget::RegExpWidget(QWidget *parent) :
    QWidget(parent)
{

    QHBoxLayout* layout = new QHBoxLayout();

    mInclude = new QLineEdit();
    mExclude = new QLineEdit();
    mSyntax = new QComboBox();
    mMode = new QComboBox();
    mMatchCase = new QCheckBox("case");
    mTest = new QToolButton();
    mIncludeLabel = new QLabel("inc");
    mExcludeLabel = new QLabel("exc");

    mTest->setText("test");

    {
        QStringList items;
        items << "regexp" << "wildcard" << "text";
        mSyntax->setModel(new QStringListModel(items,mSyntax));
    }
    {
        QStringList items;
        items << "include" << "exclude" << "inc. and exc.";
        mMode->setModel(new QStringListModel(items,mSyntax));
    }

    layout->addWidget(mIncludeLabel);
    layout->addWidget(mInclude);
    layout->addWidget(mExcludeLabel);
    layout->addWidget(mExclude);
    layout->addWidget(mSyntax);
    layout->addWidget(mMode);
    layout->addWidget(mMatchCase);
    //layout->addWidget(mTest);

    setLayout(layout);

    layout->setMargin(0);

    connect(mInclude,SIGNAL(returnPressed()),this,SLOT(onReturnPressed()));
    connect(mExclude,SIGNAL(returnPressed()),this,SLOT(onReturnPressed()));
    connect(mTest,SIGNAL(clicked()),this,SLOT(onButtonClicked()));
    connect(mMode,SIGNAL(currentIndexChanged(int)),this,SLOT(onIncludeChanged(int)));
    onIncludeChanged(mMode->currentIndex());
}

QString RegExpWidget::include() const
{
    return mInclude->text();
}

QString RegExpWidget::exclude() const
{
    return mExclude->text();
}

int RegExpWidget::syntax() const
{
    return mSyntax->currentIndex();
}

int RegExpWidget::mode() const
{
    return mMode->currentIndex();
}

bool RegExpWidget::matchCase() const
{
    return mMatchCase->isChecked();
}

void RegExpWidget::setInclude(const QString value)
{
    mInclude->setText(value);
}

void RegExpWidget::setExclude(const QString &value)
{
    mExclude->setText(value);
}

void RegExpWidget::setSyntax(int value)
{
    mSyntax->setCurrentIndex(value);
}

void RegExpWidget::setMode(int value)
{
    mMode->setCurrentIndex(value);
}

void RegExpWidget::setMatchCase(bool value)
{
    mMatchCase->setChecked(value);
}

QVariant RegExpWidget::serialize() const
{
    QVariantMap r;
    r["include"] = include();
    r["exclude"] = exclude();
    r["syntax"] = syntax();
    r["mode"] = mode();
    r["matchCase"] = matchCase();
    return r;
}

void RegExpWidget::deserialize(const QVariant &value)
{
    QVariantMap r = value.toMap();
    setInclude(r["include"].toString());
    setExclude(r["exclude"].toString());
    setSyntax(r["syntax"].toInt());
    setMode(r["mode"].toInt());
    setMatchCase(r["matchCase"].toBool());
}

void RegExpWidget::onIncludeChanged(int index) {
    Q_UNUSED(index);
#if 0
    mIncludeLabel->setVisible(index == RegExp::IncludeAndExclude);
    mExcludeLabel->setVisible(index == RegExp::IncludeAndExclude);

    mInclude->setVisible(index == RegExp::Include || index == RegExp::IncludeAndExclude);
    mExclude->setVisible(index == RegExp::Exclude || index == RegExp::IncludeAndExclude);
#endif
}

void RegExpWidget::onButtonClicked()
{
    emit buttonClicked(mInclude->text(), mExclude->text(), mSyntax->currentIndex(), mMode->currentIndex(), mMatchCase->isChecked());
}

void RegExpWidget::onReturnPressed()
{
    emit returnPressed(mInclude->text(), mExclude->text(), mSyntax->currentIndex(), mMode->currentIndex(), mMatchCase->isChecked());
}
