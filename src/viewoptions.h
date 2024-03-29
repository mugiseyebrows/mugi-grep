// DO NOT EDIT ** This file was generated by mugicpp ** DO NOT EDIT
#ifndef VIEWOPTIONS_H
#define VIEWOPTIONS_H
#include <QMetaType>

class ViewOptions {

public:
    ViewOptions();
    ViewOptions(bool search, bool filter, bool display, bool navigate, bool cache);
    void toggleSearch();
    void toggleFilter();
    void toggleDisplay();
    void toggleNavigate();
    void toggleCache();
    bool all() const;
    void setAll(bool value);
    bool search() const;
    void setSearch(bool value);
    bool filter() const;
    void setFilter(bool value);
    bool display() const;
    void setDisplay(bool value);
    bool navigate() const;
    void setNavigate(bool value);
    bool cache() const;
    void setCache(bool value);

protected:
    bool mSearch;
    bool mFilter;
    bool mDisplay;
    bool mNavigate;
    bool mCache;
};

Q_DECLARE_METATYPE(ViewOptions)

#endif // VIEWOPTIONS_H
