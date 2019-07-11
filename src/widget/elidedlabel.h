#ifndef ELIDEDLABEL_H
#define ELIDEDLABEL_H

#include <QWidget>

class ElidedLabel : public QWidget
{
    Q_OBJECT
public:
    explicit ElidedLabel(QWidget *parent = nullptr);

    virtual void paintEvent(QPaintEvent *event);

    QSize minimumSizeHint() const override;

    QSize sizeHint() const override;

    void setText(const QString& text);
    QString text() const;

    void setTextElideMode(Qt::TextElideMode mode);
    Qt::TextElideMode textElideMode() const;

protected:
    QString mText;
    Qt::TextElideMode mTextElideMode;

signals:

public slots:
};

#endif // ELIDEDLABEL_H
