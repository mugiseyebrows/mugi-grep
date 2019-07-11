#ifndef REGEXPBASEINPUT_H
#define REGEXPBASEINPUT_H

#include <QWidget>

class QLineEdit;
class QLabel;
class QCheckBox;

class RegExpBaseInput : public QWidget
{
    Q_OBJECT
public:
    enum Mode {
        Simple,
        Expert
    };
    explicit RegExpBaseInput(int inputs, const QStringList& labels, QCheckBox *case_, QWidget *parent = nullptr);

    void setMode(Mode mode);
    Mode mode() const;

    virtual void modeChanged() = 0;

protected:

    QStringList exps() const;
    void setExps(const QStringList& exps);

    QCheckBox* mCase;
    QList<QLineEdit*> mInputs;
    QList<QLabel*> mLabels;
    Mode mMode;
signals:
    void returnPressed();
public slots:
    
};

#endif // REGEXPBASEINPUT_H
