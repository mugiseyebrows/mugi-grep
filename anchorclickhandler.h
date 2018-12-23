#ifndef ANCHORCLICKHANDLER_H
#define ANCHORCLICKHANDLER_H

#include <QObject>
#include <QUrl>

class QTextBrowser;

class AnchorClickHandler : public QObject
{
    Q_OBJECT
public:
    explicit AnchorClickHandler(QObject *parent = nullptr);

    void setBrowser(QTextBrowser* browser);

protected:
    QUrl mQueued;

signals:
    void setEditor();
    void editorSet();
public slots:
    void onAnchorClicked(QUrl);

    void onSetEditor();
};

#endif // ANCHORCLICKHANDLER_H
