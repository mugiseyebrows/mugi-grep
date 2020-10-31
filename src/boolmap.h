#ifndef BOOLMAP_H
#define BOOLMAP_H

#include <QMap>

class BoolMap {
public:
    BoolMap() {

    }

    BoolMap& fill(int begin, int end, bool value) {
        for(int i=begin;i<=end;i++) {
            mMap[i] = value;
        }
        return *this;
    }

    BoolMap& true_(int begin, int end) {
        return fill(begin, end, true);
    }
    BoolMap& false_(int begin, int end) {
        return fill(begin, end, false);
    }
    QMap<int,bool> map() {
        return mMap;
    }

    QMap<int,bool> mMap;
};


#endif // BOOLMAP_H
