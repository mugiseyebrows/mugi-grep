#include "struct/searchdata.h"

SearchData::SearchData() {

}

SearchData::SearchData(QString path, RegExpPath filter, bool notBinary, RegExp search,
                       QStringList files, int linesBefore, int linesAfter,
                       int filesFiltered, int dirsFiltered)
    : path(path), filter(filter), notBinary(notBinary),
      search(search), files(files), complete(0), linesBefore(linesBefore), linesAfter(linesAfter),
      filesFiltered(filesFiltered), dirsFiltered(dirsFiltered)
{

}
