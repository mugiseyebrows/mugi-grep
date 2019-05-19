#ifndef SEARCHID_H
#define SEARCHID_H

class SearchId
{
public:
    static SearchId* instance();

    int next();

protected:
    static SearchId* mInstance;
    int mId;
    SearchId();
};

#endif // SEARCHID_H
