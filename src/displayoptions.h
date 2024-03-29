// DO NOT EDIT ** This file was generated by mugicpp ** DO NOT EDIT
#ifndef DISPLAYOPTIONS_H
#define DISPLAYOPTIONS_H

class DisplayOptions {

public:
    DisplayOptions();
    DisplayOptions(int linesBefore, int linesAfter, bool fileName, bool lineNumber, bool wholeLine,
                   bool context, bool signature);
    int linesBefore() const;
    void setLinesBefore(int value);
    int linesAfter() const;
    void setLinesAfter(int value);
    bool fileName() const;
    void setFileName(bool value);
    bool lineNumber() const;
    void setLineNumber(bool value);
    bool wholeLine() const;
    void setWholeLine(bool value);
    bool context() const;
    void setContext(bool value);
    bool signature() const;
    void setSignature(bool value);

protected:
    int mLinesBefore;
    int mLinesAfter;
    bool mFileName;
    bool mLineNumber;
    bool mWholeLine;
    bool mContext;
    bool mSignature;
};

#endif // DISPLAYOPTIONS_H
