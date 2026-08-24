#include "statwidget.h"
#include "ui_statwidget.h"

#include <QStandardItemModel>


StatWidget::StatWidget(const QList<QPair<QString, qint64>>& stat, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StatWidget)
{
    ui->setupUi(this);

    QStandardItemModel* model = new QStandardItemModel(0, 2);

    for(int row = 0; row < stat.size(); row++) {
#if 0
        model->setData(model->index(row, 0), stat[row].first);
        model->setData(model->index(row, 1), stat[row].second);
#endif
        QStandardItem* col0 = new QStandardItem();
        QStandardItem* col1 = new QStandardItem();
        col0->setData(stat[row].first, Qt::DisplayRole);
        col1->setData(stat[row].second, Qt::DisplayRole);
        col0->setCheckable(true);
        model->appendRow({col0, col1});
    }
    model->setHorizontalHeaderLabels({"Ext", "Size"});

    ui->table->setModel(model);

    connect(ui->include, &QPushButton::clicked, this, onInclude);
    connect(ui->exclude, &QPushButton::clicked, this, onExclude);
}

void StatWidget::onInclude() {
    QStringList vs = checked();
    if (vs.isEmpty()) {
        return;
    }
    emit clicked(Include, vs.join("|"));
}

void StatWidget::onExclude() {
    QStringList vs = checked();
    if (vs.isEmpty()) {
        return;
    }
    emit clicked(Exclude, vs.join("|"));
}

StatWidget::~StatWidget()
{
    delete ui;
}

QStringList StatWidget::checked() const
{
    QStringList res;
    QAbstractItemModel* model = ui->table->model();
    for(int row=0;row<model->rowCount();row++) {
        QModelIndex index = model->index(row, 0);

        int state = index.data(Qt::CheckStateRole).toInt();
        if (state == Qt::Checked) {
            res.append(index.data().toString());
        }
    }
    return res;
}
