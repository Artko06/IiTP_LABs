#ifndef SET_H
#define SET_H

#include "RBTree.h"

namespace my {

template<typename KT>
class set : public RBTree<KT>
{
public:
    using RBTree<KT>::RBTree;
    using Key = KT;
    using key_type = Key;
    using typename RBTree<KT>::value_type;
    using typename RBTree<value_type>::reference;
    using typename RBTree<value_type>::const_reference;
    using typename RBTree<value_type>::iterator;
    using typename RBTree<value_type>::const_iterator;
    using typename RBTree<value_type>::size_type;

    std::pair<iterator, bool> insert(const value_type &value)
    {
        return this->insertInTree(value, std::make_pair(false, false));
    };

    iterator find(const Key &key) const
    {
        iterator new_iter(this->content(key).first);
        return new_iter;
    };

    bool contains(const Key &key) const { return this->content(key).second; };
};

} // namespace my

template class my::set<int32_t>;

#endif // SET_H
