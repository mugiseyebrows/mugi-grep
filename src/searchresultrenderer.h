#ifndef SEARCHRESULTRENDERER_H
#define SEARCHRESULTRENDERER_H

#include <QObject>

#include "displayoptionswidget.h"

#include "searchhits.h"
#include "regexpreplacement.h"
#include "replaceparams.h"

class ColoredLine;

class SearchTab;

class SearchResultRenderer : public QObject
{
    Q_OBJECT
public:

    explicit SearchResultRenderer(QObject *parent = nullptr);
    void setTab(SearchTab* tab);
    void append(const SearchHits &hits);

    ReplaceParams replaceParams();

    void setDarkMode(bool darkMode);

    void setBaseColor(const QString& color);

    static void testTokenize();
protected:
    SearchTab* mTab;
    bool mZebra;

    bool mDarkMode;

    QString mBaseColor;

    QStringList toHtmlSpans(const ColoredLine &coloredLine, const QStringList &backgroundColors);
    QStringList fileNameLineNumber(bool showFileName, bool showLineNumber, const QString &relativePath, const QString &href, int lineNumber);
    QMap<int, bool> doZebra(int before, int after, const QList<int> &matched);
    void appendSearch(const SearchHits &hits);
    void appendPreview(const SearchHits &hits);
    void appendReplace(const SearchHits &hits);
signals:

protected slots:
    void onOptionsChanged();
};

#endif // SEARCHRESULTRENDERER_H
