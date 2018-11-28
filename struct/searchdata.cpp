#include "struct/searchdata.h"

SearchData::SearchData() {

}

SearchData::SearchData(QString path, RegExpPath filter, bool notBinary, RegExp search, QStringList files, int linesBefore, int linesAfter)
    : path(path), filter(filter), notBinary(notBinary),
      search(search), files(files), complete(0), linesBefore(linesBefore), linesAfter(linesAfter)
{

}
