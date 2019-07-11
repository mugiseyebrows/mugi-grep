#include "searchid.h"

SearchId *SearchId::mInstance = nullptr;

SearchId *SearchId::instance()
{
    if (!mInstance) {
        mInstance = new SearchId();
    }
    return mInstance;
}

int SearchId::next()
{
    return mId++;
}

SearchId::SearchId() : mId(1)
{

}
