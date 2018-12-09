#ifndef SEARCHDATA_H
#define SEARCHDATA_H

#include "regexppath.h"
#include "regexp.h"

struct SearchData {

    SearchData();
    SearchData(QString path, RegExpPath filter, bool notBinary, RegExp search,
               QStringList files, int linesBefore, int linesAfter, int filesFiltered, int dirsFiltered);

    QString path;
    RegExpPath filter;
    bool notBinary;
    RegExp search;
    QStringList files;
    int complete;
    int linesBefore;
    int linesAfter;
    int filesFiltered;
    int dirsFiltered;
};


#endif // SEARCHDATA_H
