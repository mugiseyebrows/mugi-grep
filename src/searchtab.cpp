// DO NOT EDIT ** This file was generated by mugicpp ** DO NOT EDIT
#include "searchtab.h"
#include <QFont>
#include <QTextBrowser>
#include <QVBoxLayout>

SearchTab::SearchTab(QWidget* parent) : QWidget(parent), mMode(0) {

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
    mDisplayOptionsWidget = new DisplayOptionsWidget();
    mRenderer = new SearchResultRenderer();
    layout->addWidget(mTextBrowser);
    layout->addWidget(mDisplayOptionsWidget);
    setLayout(layout);
    mRenderer->setTab(this);
}
int SearchTab::mode() const {
    return mMode;
}
void SearchTab::setMode(int value) {
    mMode = value;
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
DisplayOptionsWidget* SearchTab::displayOptionsWidget() const {
    return mDisplayOptionsWidget;
}
void SearchTab::setDisplayOptionsWidget(DisplayOptionsWidget* value) {
    mDisplayOptionsWidget = value;
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
    int linesBefore = mDisplayOptionsWidget->linesBefore();
    int linesAfter = mDisplayOptionsWidget->linesAfter();
    mHits.read(linesBefore, linesAfter);
}
void SearchTab::trigRerender() {
    mDisplayOptionsWidget->trigChanged();
}
SearchParams& SearchTab::params() {
    return mParams;
}
SearchHits& SearchTab::hits() {
    return mHits;
}
DisplayOptions SearchTab::displayOptions() const {
    return mDisplayOptionsWidget->options();
}
void SearchTab::setDisplayOptions(const DisplayOptions& value) {
    mDisplayOptionsWidget->setOptions(value);
}
