#ifndef SEARCHRESULTRENDERER_H
#define SEARCHRESULTRENDERER_H

#include <QObject>

class Colors;
class ColoredLine;

class SearchTab;
class SearchHitsWithContext;
class ReplaceParams;

class SearchResultRenderer : public QObject
{
    Q_OBJECT
public:

    explicit SearchResultRenderer(QObject *parent = nullptr);
    void setTab(SearchTab* tab);
    void append(const SearchHitsWithContext &hits);

    ReplaceParams replaceParams(bool rename);

    static void testTokenize();


    static void testDoZebra();
protected:
    SearchTab* mTab;
    bool mZebra;

    QStringList toHtmlSpans(const ColoredLine &coloredLine, const QStringList &backgroundColors);
    static QMap<int, bool> doZebra(int before, int after, const QList<int> &matched, bool *initial);
    void appendSearch(const SearchHitsWithContext &hits);
    void appendPreview(const SearchHitsWithContext &hits);
    void appendReplace(const SearchHitsWithContext &hits);
    //QStringList fileNameLineNumber(const Colors &colors, bool showFileName, bool showLineNumber, const QString &relativePath, const QString &href, int lineNumber, const QString &separator);
    QStringList fileNameLineNumberContext(const Colors &colors, bool showFileName, bool showLineNumber, const QString &relativePath, const QString &href, int lineNumber, const QString &separator);
signals:

protected slots:
    void onOptionsChanged();
};

#endif // SEARCHRESULTRENDERER_H
