#include "tablebuttongroup.h"

namespace  {

QList<TableButtonImpl*> filterOrientation(const QList<TableButtonImpl*>& buttons, Qt::Orientation orientation) {
    QList<TableButtonImpl*> res;
    foreach(TableButtonImpl* button, buttons) {
        if (button->orientation() == orientation) {
            res.append(button);
        }
    }
    return res;
}

int minLeft(const QList<TableButtonImpl*>& buttons) {
    std::vector<int> lefts;
    std::transform(buttons.begin(),buttons.end(),std::back_inserter(lefts),
                   [](TableButtonImpl* button){return button->left();});
    return *std::min_element(lefts.begin(),lefts.end());
}

int maxRight(const QList<TableButtonImpl*>& buttons) {
    std::vector<int> rights;
    std::transform(buttons.begin(),buttons.end(),std::back_inserter(rights),
                   [](TableButtonImpl* button){return button->right();});
    return *std::max_element(rights.begin(),rights.end());
}

int maxBottom(const QList<TableButtonImpl*>& buttons) {
    std::vector<int> bottoms;
    std::transform(buttons.begin(),buttons.end(),std::back_inserter(bottoms),
                   [](TableButtonImpl* button){return button->bottom();});
    return *std::max_element(bottoms.begin(),bottoms.end());
}

int minTop(const QList<TableButtonImpl*>& buttons) {
    std::vector<int> tops;
    std::transform(buttons.begin(),buttons.end(),std::back_inserter(tops),
                   [](TableButtonImpl* button){return button->top();});
    return *std::min_element(tops.begin(),tops.end());
}


}

TableButtonGroup::TableButtonGroup(const QList<TableButtonImpl *> &buttons, Qt::Orientation orientation)
{
    QList<TableButtonImpl *> filtered = filterOrientation(buttons, orientation);
    append(filtered);
}

int TableButtonGroup::left() const
{
    return minLeft(*this);
}

int TableButtonGroup::right() const
{
    return maxRight(*this);
}

int TableButtonGroup::width() const {
    return right() - left();
}

int TableButtonGroup::height() const {
    return bottom() - top();
}

int TableButtonGroup::top() const
{
    return minTop(*this);
}

int TableButtonGroup::bottom() const
{
    return maxBottom(*this);
}
