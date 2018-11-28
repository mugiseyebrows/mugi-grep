#ifndef SEARCHDATA_H
#define SEARCHDATA_H

#include "regexppath.h"
#include "regexp.h"

class SearchData {
public:
    SearchData();
    SearchData(QString path, RegExpPath filter, bool notBinary, RegExp search,
               QStringList files, int linesBefore, int linesAfter);

    QString path;
    RegExpPath filter;
    bool notBinary;
    RegExp search;
    QStringList files;
    int complete;
    int linesBefore;
    int linesAfter;
};


#endif // SEARCHDATA_H
