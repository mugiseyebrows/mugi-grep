#ifndef TABLEBUTTONS_H
#define TABLEBUTTONS_H

#include <QObject>

#include "tablebutton.h"
#include "tablebuttonimpl.h"
#include <QMap>
#include <QPoint>
class QTableView;
class QHeaderView;
class QSignalMapper;

class TableButtons : public QObject
{
    Q_OBJECT
public:
    explicit TableButtons(QObject *parent = nullptr);
    ~TableButtons();

    void setView(QTableView* view);
    void clear();
    TableButton button(int id);
    void setVisible(bool visible);
    bool visible() const;

protected:

    int nextId();
    virtual void updateVertical();
    virtual void updateHorizontal();
    void create();
    bool eventFilter(QObject *object, QEvent *event);
    int posCenter(QHeaderView *header, int pos, int *index_);
    int posBorder(QHeaderView *header, int pos, int *index);
    int indexAt(QHeaderView *header, int pos);
    int posBorderShifted(QHeaderView *header, int pos, int shift, int buttonSize, int* index);
    int posCenterShifted(QHeaderView *header, int pos, int shift, int buttonSize, int *index);
    int posCenterFixed(QHeaderView *header, int index);
    int posBorderFixed(QHeaderView *header, int index);
    int posCenterFixedShifted(QHeaderView *header, int index, int shift, int buttonSize);
    int posBorderFixedShifted(QHeaderView *header, int index, int shift, int buttonSize);
    int posBorderOrCenterShifted(TableButtonImpl::ButtonPosition position, QHeaderView *header, int pos, int shift, int buttonSize, int* index);
    int posBorderOrCenterFixedShifted(TableButtonImpl::ButtonPosition position, QHeaderView *header, int index, int shift, int buttonSize);
    int fixedButtonIndex(QHeaderView *header, TableButtonImpl *button);

    QMap<int,TableButtonImpl*> mButtons;
    QTableView* mView;
    QPushButton* mButton;
    QPoint mPoint;
    bool mVisible;

signals:
    void clicked(int,int);
protected slots:
    void onSliderMoved(int);

public slots:
    virtual void update();
};

#endif // TABLEBUTTONS_H
