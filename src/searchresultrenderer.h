#ifndef SEARCHRESULTRENDERER_H
#define SEARCHRESULTRENDERER_H

#include <QObject>

#include "displayoptionswidget.h"

#include "searchhits.h"
#include "regexpreplacement.h"

class ColoredLine;

class SearchTab;

class SearchResultRenderer : public QObject
{
    Q_OBJECT
public:
    enum Mode {
        Search,
        Preview
    };

    explicit SearchResultRenderer(QObject *parent = nullptr);
    void setTab(SearchTab* tab);
    void append(const SearchHits &hits);
    void setMode(Mode mode);

    void setReplacement(const RegExpReplacement& value);


protected:
    SearchTab* mTab;
    bool mZebra;
    Mode mMode;
    RegExpReplacement mReplacement;

    QStringList toHtmlSpans(const ColoredLine &coloredLine, const QStringList &backgroundColors);
    QStringList fileNameLineNumber(bool showFileName, bool showLineNumber, const QString &relativePath, const QString &href, int lineNumber);
    QMap<int, bool> doZebra(int before, int after, const QList<int> &matched);
    void appendSearch(const SearchHits &hits);
    void appendPreview(const SearchHits &hits);
signals:

protected slots:
    void onOptionsChanged();
};

#endif // SEARCHRESULTRENDERER_H
