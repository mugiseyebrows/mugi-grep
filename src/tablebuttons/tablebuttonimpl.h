#ifndef TABLEBUTTONIMPL_H
#define TABLEBUTTONIMPL_H
#include <QIcon>
#include <QPoint>
#include <QPushButton>
#include <QSize>
#include <QTableView>

class TableButtonImpl {
public:
    enum ButtonType {
        ButtonTypeVariable,
        ButtonTypeFixed,
        ButtonTypePrepend,
        ButtonTypeAppend,
    };
    enum ButtonPosition {
        ButtonPositionInside,
        ButtonPositionBetween,
    };
    enum UpdateProp {
        Text = 0x1,
        Size = 0x2,
        Icon = 0x4,
        IconSize = 0x8,
        All = Text + Size + Icon + IconSize,
    };
    TableButtonImpl(int id);
    ~TableButtonImpl();
    void fixed(int index);
    void variable();
    void inside();
    void between();
    void insert();
    void remove();
    void append();
    void prepend();
    void horizontal();
    void vertical();
    void text(const QString& text);
    void icon(const QIcon& icon);
    void size(int w, int h);
    void size(const QSize& size);
    void iconSize(int w, int h);
    void iconSize(const QSize& size);
    void offset(int x, int y);
    void offset(const QPoint& offset);
    QPushButton* button() const;
    int id() const;
    QPoint offset() const;
    QSize size() const;
    QSize iconSize() const;
    Qt::Orientation orientation() const;
    QIcon icon() const;
    int index() const;
    ButtonType type() const;
    ButtonPosition position() const;
    QString text() const;
    int top() const;
    int left() const;
    int bottom() const;
    int right() const;
    int currentIndex() const;
    void setCurrentIndex(int currentIndex);
    QPushButton* create(QTableView* view);

protected:
    void update(int updateProp);
    QPushButton* mButton;
    int mId;
    QPoint mOffset;
    QSize mSize;
    QSize mIconSize;
    Qt::Orientation mOrientation;
    QIcon mIcon;
    int mIndex;
    ButtonType mType;
    ButtonPosition mPosition;
    QString mText;
    int mCurrentIndex;
};
#endif // TABLEBUTTONIMPL_H
