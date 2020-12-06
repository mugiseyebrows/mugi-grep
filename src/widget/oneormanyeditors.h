#ifndef ONEORMANYEDITORS_H
#define ONEORMANYEDITORS_H

#include <QDialog>

namespace Ui {
class OneOrManyEditors;
}

class OneOrManyEditors : public QDialog
{
    Q_OBJECT

public:
    explicit OneOrManyEditors(QWidget *parent = nullptr);
    ~OneOrManyEditors();

    bool one() const;

    bool many() const;

private:
    Ui::OneOrManyEditors *ui;
};

#endif // ONEORMANYEDITORS_H
