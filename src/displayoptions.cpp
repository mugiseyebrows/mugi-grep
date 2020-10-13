// DO NOT EDIT ** This file was generated by mugicpp ** DO NOT EDIT
#include "displayoptions.h"

DisplayOptions::DisplayOptions() : mLinesBefore(0), mLinesAfter(0) {
}
DisplayOptions::DisplayOptions(int linesBefore, int linesAfter, bool fileName, bool lineNumber,
                               bool wholeLine)
    : mLinesBefore(linesBefore), mLinesAfter(linesAfter), mFileName(fileName),
      mLineNumber(lineNumber), mWholeLine(wholeLine) {
}
int DisplayOptions::linesBefore() const {
    return mLinesBefore;
}
void DisplayOptions::setLinesBefore(int value) {
    mLinesBefore = value;
}
int DisplayOptions::linesAfter() const {
    return mLinesAfter;
}
void DisplayOptions::setLinesAfter(int value) {
    mLinesAfter = value;
}
bool DisplayOptions::fileName() const {
    return mFileName;
}
void DisplayOptions::setFileName(bool value) {
    mFileName = value;
}
bool DisplayOptions::lineNumber() const {
    return mLineNumber;
}
void DisplayOptions::setLineNumber(bool value) {
    mLineNumber = value;
}
bool DisplayOptions::wholeLine() const {
    return mWholeLine;
}
void DisplayOptions::setWholeLine(bool value) {
    mWholeLine = value;
}
