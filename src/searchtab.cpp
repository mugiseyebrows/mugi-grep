// DO NOT EDIT ** This file was generated by mugicpp ** DO NOT EDIT
#include "searchtab.h"
#include <QFont>
#include <QTextBrowser>
#include <QVBoxLayout>

SearchTab::SearchTab(QWidget* parent) : QWidget(parent) {

    QVBoxLayout* layout = new QVBoxLayout();
    mTextBrowser = new QTextBrowser();
#ifdef Q_OS_LINUX
    QFont font("Liberation Mono", 11, QFont::Normal);
    mTextBrowser->setFont(font);
#endif
#ifdef Q_OS_WIN
    QFont font("Courier New", 12, QFont::Normal);
    mTextBrowser->setFont(font);
#endif
    mTextBrowser->setOpenLinks(false);
    mDisplayOptions = new DisplayOptionsWidget();
    mRenderer = new SearchResultRenderer();
    layout->addWidget(mTextBrowser);
    layout->addWidget(mDisplayOptions);
    setLayout(layout);
    mRenderer->setTab(this);
}
void SearchTab::setParams(const SearchParams& value) {
    mParams = value;
}
void SearchTab::setHits(const SearchHits& value) {
    mHits = value;
}
QTextBrowser* SearchTab::textBrowser() const {
    return mTextBrowser;
}
void SearchTab::setTextBrowser(QTextBrowser* value) {
    mTextBrowser = value;
}
DisplayOptionsWidget* SearchTab::displayOptions() const {
    return mDisplayOptions;
}
void SearchTab::setDisplayOptions(DisplayOptionsWidget* value) {
    mDisplayOptions = value;
}
SearchResultRenderer* SearchTab::renderer() const {
    return mRenderer;
}
void SearchTab::setRenderer(SearchResultRenderer* value) {
    mRenderer = value;
}
void SearchTab::append(const SearchHits& hits) {
    if (hits.isEmpty()) {
        return;
    }
    int size = mHits.size();
    mHits.append(hits);
    read();
    mRenderer->append(mHits.mid(size));
}
void SearchTab::read() {
    int linesBefore = mDisplayOptions->linesBefore();
    int linesAfter = mDisplayOptions->linesAfter();
    mHits.read(linesBefore, linesAfter);
}
void SearchTab::trigRerender() {
    mDisplayOptions->trigChanged();
}
SearchParams& SearchTab::params() {
    return mParams;
}
SearchHits& SearchTab::hits() {
    return mHits;
}
