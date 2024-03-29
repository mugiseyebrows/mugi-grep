// DO NOT EDIT ** This file was generated by mugicpp ** DO NOT EDIT
#include "searchtab.h"
#include <QFont>
#include <QTextBrowser>
#include <QVBoxLayout>

SearchTab::SearchTab(QWidget* parent)
    : QWidget(parent), mUpdated(QDateTime::currentDateTime()), mMode(Mode::Search) {

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
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);
    mRenderer->setTab(this);
}
Mode SearchTab::mode() const {
    return mMode;
}
void SearchTab::setParams(const SearchParams& value) {
    mParams = value;
}
void SearchTab::setHits(const SearchHits& value) {
    mHits = value;
}
void SearchTab::setNameHits(const SearchNameHits& value) {
    mNameHits = value;
}
QDateTime SearchTab::updated() const {
    return mUpdated;
}
void SearchTab::setUpdated(const QDateTime& value) {
    mUpdated = value;
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
void SearchTab::append(const SearchHits& hits, const SearchNameHits& nameHits) {
    mUpdated = QDateTime::currentDateTime();
    if (hits.isEmpty()) {
        return;
    }
    int size = mHits.size();
    mHits.append(hits);
    mNameHits.append(nameHits);
    read();
    mRenderer->append(mHits.mid(size));
}
void SearchTab::read() {
    DisplayOptions options = mDisplayOptionsWidget->options();
    mHits.read(options.linesBefore(), options.linesAfter());
}
void SearchTab::trigRerender() {
    mDisplayOptionsWidget->trigChanged();
}
void SearchTab::setMode(Mode value) {
    if (mMode == value) {
        return;
    }
    mMode = value;
    trigRerender();
}
SearchParams& SearchTab::params() {
    return mParams;
}
SearchHits& SearchTab::hits() {
    return mHits;
}
SearchNameHits& SearchTab::nameHits() {
    return mNameHits;
}
DisplayOptions SearchTab::displayOptions() const {
    return mDisplayOptionsWidget->options();
}
void SearchTab::setDisplayOptions(const DisplayOptions& value) {
    mDisplayOptionsWidget->setOptions(value);
}
ReplaceParams SearchTab::replaceParams(bool renameFiles) {
    return mRenderer->replaceParams(renameFiles);
}
QString SearchTab::toPlainText() const {
    return mTextBrowser->toPlainText();
}
QString SearchTab::toHtml() const {
    return mTextBrowser->toHtml();
}
void SearchTab::setViewOptions(const ViewOptions& options) {
    mDisplayOptionsWidget->setVisible(options.display());
}
