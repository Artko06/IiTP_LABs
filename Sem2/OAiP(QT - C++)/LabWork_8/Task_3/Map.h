#ifndef MAP_H
#define MAP_H

#include "RBTree.h"
#include <stdexcept>

namespace my {

template<typename KT, typename T>
class map : public RBTree<std::pair<KT, T>>
{
public:
    using RBTree<std::pair<KT, T>>::RBTree;
    using Key = KT;
    using key_type = Key;
    using mapped_type = T;
    using typename RBTree<std::pair<KT, T>>::value_type;
    using typename RBTree<value_type>::const_reference;
    using typename RBTree<value_type>::iterator;
    using typename RBTree<value_type>::const_iterator;
    using typename RBTree<value_type>::size_type;

    std::pair<iterator, bool> insert(const value_type &value)
    {
        return this->insertInTree(value, std::make_pair(false, false));
    }

    std::pair<iterator, bool> insert(const Key &key, const T &obj)
    {
        value_type value = std::make_pair(key, obj);
        return this->insertInTree(value, std::make_pair(false, false));
    };

    bool contains(const Key &key) const
    {
        value_type value = std::make_pair(key, T());
        return this->content(value).second;
    }

    iterator find(const Key &key) const
    {
        std::pair<KT, T> temp;
        for (auto i = this->begin(); i != this->end(); ++i) {
            temp = *i;
            if (key == temp.first) {
                return i;
            }
        }
        return this->end();
    }

    bool erace(const Key &key)
    {
        iterator temp = find(key);
        if (temp == this->end()) {
            return false;
        } else {
            this->erase(temp);
            return true;
        }
    }

    T &at(const Key &key)
    {
        value_type value = std::make_pair(key, T());
        std::pair<TreeNode<value_type> *, bool> new_value = this->content(value);
        if (new_value.second != true) {
            throw std::out_of_range("Key not found.");
        } else {
            return new_value.first->value.second;
        }
    };

    T at(const Key &key) const
    {
        value_type value = std::make_pair(key, T());
        std::pair<TreeNode<value_type> *, bool> new_value = this->content(value);
        if (new_value.second != true) {
            throw std::out_of_range("Key not found.");
        } else {
            return new_value.first->value.second;
        }
    };

    T &operator[](const Key &key)
    {
        try {
            return at(key);
        } catch (const std::out_of_range &e) {
            insert(key, T());
            return at(key);
        }
    };

    T operator[](const Key &key) const { return at(key); };
};

} // namespace my

template class my::map<int32_t, QString>;

#endif // MAP_H
