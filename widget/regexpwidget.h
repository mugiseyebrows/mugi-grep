#ifndef REGEXPWIDGET_H
#define REGEXPWIDGET_H

#include <QWidget>

class QLineEdit;
class QComboBox;
class QCheckBox;
class QPushButton;
class QToolButton;
class QLabel;

class RegExpWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RegExpWidget(QWidget *parent = 0);
    
    /** @todo setters */
    QString include() const;
    QString exclude() const;
    int syntax() const;
    int mode() const;
    bool matchCase() const;

    void setInclude(const QString value);
    void setExclude(const QString& value);
    void setSyntax(int value);
    void setMode(int value);
    void setMatchCase(bool value);

    QVariant serialize() const;
    void deserialize(const QVariant& value);

protected:
    QLineEdit* mInclude;
    QLineEdit* mExclude;
    QLabel* mIncludeLabel;
    QLabel* mExcludeLabel;
    QComboBox* mSyntax;
    QComboBox* mMode;
    QCheckBox* mMatchCase;
    //QPushButton* mPushButton;
    QToolButton* mTest;

signals:
    void returnPressed(QString, QString, int, int, bool);
    void buttonClicked(QString, QString, int, int, bool);

protected slots:
    void onButtonClicked();
    void onReturnPressed();
    void onIncludeChanged(int index);
};

#endif // REGEXPWIDGET_H
