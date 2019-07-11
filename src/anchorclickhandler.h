#ifndef ANCHORCLICKHANDLER_H
#define ANCHORCLICKHANDLER_H

#include <QObject>
#include <QUrl>
#include <QPoint>

class QTextBrowser;

class AnchorClickHandler : public QObject
{
    Q_OBJECT
public:
    explicit AnchorClickHandler(QObject *parent = nullptr);

    void connectBrowser(QTextBrowser* browser);

protected:
    //QUrl mQueued;

signals:
    //void setEditor();
    //void editorSet();
public slots:

    void onSetEditor();
protected slots:
    void onAnchorClicked(QUrl);
    void onCustomContextMenuRequested(QPoint point);
};

#endif // ANCHORCLICKHANDLER_H
