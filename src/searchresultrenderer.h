#ifndef SEARCHRESULTRENDERER_H
#define SEARCHRESULTRENDERER_H

#include <QObject>

#include "displayoptionswidget.h"
//#include "searchtab.h"
#include "searchhits.h"

class ColoredLine;

class SearchTab;

class SearchResultRenderer : public QObject
{
    Q_OBJECT
public:
    explicit SearchResultRenderer(QObject *parent = nullptr);
    void setTab(SearchTab* tab);

    void append(const SearchHits &hits);

protected:
    SearchTab* mTab;

    QStringList toHtmlSpans(const ColoredLine &coloredLine, const QStringList &backgroundColors);
    QStringList fileNameLineNumber(bool showFileName, bool showLineNumber, const QString &relativePath, const QString &href, int lineNumber);
signals:

protected slots:
    void onOptionsChanged();
};

#endif // SEARCHRESULTRENDERER_H
