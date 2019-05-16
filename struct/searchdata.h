#ifndef SEARCHDATA_H
#define SEARCHDATA_H

#include "regexppath.h"
#include "regexp.h"

struct SearchData {

    SearchData();
    SearchData(int action, QString path, RegExpPath filter, bool notBinary, RegExp search,
               QStringList files, int linesBefore, int linesAfter, int filesFiltered, int dirsFiltered, const QString& replacement);

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
    int action;
    QString replacement;
};


#endif // SEARCHDATA_H
