#include "RBTree.h"
#include <stdexcept>

template<typename K>
RBTree<K>::RBTree(const RBTree &other)
{
    iterator oth_iter = other.begin();
    while (oth_iter.node_ != nullptr) {
        insertInTree(*(oth_iter++), std::make_pair(false, false));
    }
}

template<typename K>
RBTree<K>::RBTree(RBTree &&other) noexcept
{
    std::swap(tree_node_, other.tree_node_);
    std::swap(number_of_elements_, other.number_of_elements_);
}

template<typename K>
RBTree<K>::~RBTree()
{
    clear();
}

template<typename K>
typename RBTree<K>::iterator RBTree<K>::begin() const
{ // Самый левый элемент
    iterator smallest_key(tree_node_);
    while (smallest_key.node_ != nullptr) {
        --smallest_key;
    }
    return ++smallest_key;
}

template<typename K>
typename RBTree<K>::iterator RBTree<K>::end() const
{ // nullptr за самым правым элементом
    iterator biggest_key(tree_node_);
    while (biggest_key.node_ != nullptr) {
        ++biggest_key;
    }
    return biggest_key;
}

/*template<typename K>
typename RBTree<K>::const_iterator RBTree<K>::cbegin() const
{
    const_iterator smallest_key(tree_node_);
    while (smallest_key.node_ != nullptr) {
        --smallest_key;
    }
    return ++smallest_key;
}*/

template<typename K>
typename RBTree<K>::const_iterator RBTree<K>::cend() const
{
    const_iterator biggest_key(tree_node_);
    while (biggest_key.node_ != nullptr) {
        ++biggest_key;
    }

    return biggest_key;
}

template<typename K>
bool RBTree<K>::empty() const
{
    return (tree_node_ == nullptr) ? true : false;
}

template<typename K>
typename RBTree<K>::size_type RBTree<K>::size() const
{
    return number_of_elements_;
}

template<typename K>
void RBTree<K>::swap(RBTree &other)
{
    std::swap(tree_node_, other.tree_node_);
    std::swap(number_of_elements_, other.number_of_elements_);
}

template<typename K>
void RBTree<K>::clear()
{
    ClearRecursive(tree_node_);
    tree_node_ = nullptr;
    number_of_elements_ = 0;
}

template<typename K>
void RBTree<K>::erase(iterator pos)
{
    if (pos.node_ == nullptr) {
        throw std::out_of_range("This element does not exist");
    } else if (pos.node_->left != nullptr && pos.node_->right != nullptr) {
        DeleteTwoChild(pos);
    } else if (pos.node_->right == nullptr && pos.node_->left == nullptr) {
        DeletingWithoutChild(pos);
    } else {
        DeleteOneChild(pos);
    }
}

template<typename K>
void RBTree<K>::merge(RBTree &other)
{
    iterator oth_iter = other.begin();
    while (oth_iter.node_ != nullptr) {
        insertInTree((oth_iter++).node_->value, std::make_pair(false, false));
    }
}

template<typename K>
std::pair<TreeNode<typename RBTree<K>::value_type> *, bool> RBTree<K>::content(
    const value_type &value) const
{
    TreeNode<value_type> *node = tree_node_;
    while (node != nullptr) {
        if (IsItMore(value, node->value)) {
            node = node->right;
        } else if (IsItLess(value, node->value)) {
            node = node->left;
        } else if (AreTheyEqual(value, node->value)) {
            return std::make_pair(node, true);
        }
    }

    return std::make_pair(node, false);
}

template<typename K>
std::pair<typename RBTree<K>::iterator, bool> RBTree<K>::insertInTree(const value_type &value,
                                                                      std::pair<bool, bool> mode)
{
    if (tree_node_ == nullptr) {
        tree_node_ = CreateNode(value);
        iterator new_iter(tree_node_);
        InsertColoringCaseOne(new_iter);

        return std::make_pair(new_iter, true);
    } else {
        return InsertOtherNodes(value, mode);
    }
}

template<typename K>
bool RBTree<K>::equalityOfClasses(const RBTree &other)
{
    return tree_node_ == other.tree_node_;
}

template<typename K>
void RBTree<K>::ClearRecursive(TreeNode<value_type> *&node) const
{
    if (node != nullptr) {
        ClearRecursive(node->right);
        ClearRecursive(node->left);
        delete node;
        node = nullptr;
    }
}

template<typename K>
std::pair<typename RBTree<K>::iterator, bool> RBTree<K>::InsertOtherNodes(const value_type &value,
                                                                          std::pair<bool, bool> mode)
{ // mode.first - нужно ли обновление эелементов mode.second -
    // можно ли добавлять одинаковые
    TreeNode<value_type> *node = tree_node_;
    bool flag = false, inserted = false;
    iterator new_iter;
    while (flag != true) {
        if (IsItMore(value, node->value)) {
            if (node->right == nullptr) {
                new_iter = CreatingANode(node, value, true);
                flag = inserted = true;
            } else {
                node = node->right;
            }
        } else if (IsItLess(value, node->value)) {
            if (node->left == nullptr) {
                new_iter = CreatingANode(node, value, false);
                flag = inserted = true;
            } else {
                node = node->left;
            }
        } else if (mode.first == true && AreTheyEqual(value, node->value)) {
            new_iter = node;
            node->value = value;
            flag = true;
        } else if (mode.second == true && AreTheyEqual(value, node->value)) {
            new_iter = InsertEquals(node, value);
            InsertColoringCaseOne(new_iter);
            FindRoot();
            flag = inserted = true;
        } else {
            new_iter = node;
            flag = true;
        }
    }
    return std::make_pair(new_iter, inserted);
}

template<typename K>
typename RBTree<K>::iterator RBTree<K>::InsertEquals(TreeNode<value_type> *node,
                                                     const value_type &value)
{
    iterator new_iter(CreateNode(value));

    new_iter.node_->right = node->right;
    new_iter.node_->parent = node;
    if (node->right != nullptr) {
        node->right->parent = new_iter.node_;
    }
    node->right = new_iter.node_;

    return new_iter;
}

template<typename K>
TreeNode<typename RBTree<K>::value_type> *RBTree<K>::CreateNode(const value_type &value)
{
    TreeNode<value_type> *new_node = new TreeNode<value_type>;
    new_node->parent = new_node->left = new_node->right = nullptr;
    new_node->color = RED;
    new_node->value = value;
    ++number_of_elements_;
    return new_node;
}

template<typename K>
typename RBTree<K>::iterator RBTree<K>::CreatingANode(TreeNode<value_type> *node,
                                                      const value_type &value,
                                                      bool right)
{
    iterator new_iter(CreateNode(value));
    right ? node->right = new_iter.node_ : node->left = new_iter.node_;
    new_iter.node_->parent = node;
    InsertColoringCaseOne(new_iter);
    FindRoot();
    return new_iter;
}

template<typename K>
typename RBTree<K>::iterator RBTree<K>::Grandparent(iterator &iter)
{
    iterator new_iter;
    if (iter.node_ != nullptr && iter.node_->parent != nullptr) {
        new_iter = iter.node_->parent->parent;
    }
    return new_iter;
}

template<typename K>
typename RBTree<K>::iterator RBTree<K>::Sibling(iterator &iter)
{
    iterator bro(iter);
    if (iter.node_ == iter.node_->parent->left) {
        bro.node_ = bro.node_->parent->right;
    } else {
        bro.node_ = bro.node_->parent->left;
    }
    return bro;
}

template<typename K>
typename RBTree<K>::iterator RBTree<K>::Uncle(iterator &iter)
{
    iterator grp = Grandparent(iter);
    if (grp.node_ != nullptr) {
        if (iter.node_->parent == grp.node_->left) {
            grp.node_ = grp.node_->right;
        } else {
            grp.node_ = grp.node_->left;
        }
    }

    return grp;
}

template<typename K>
void RBTree<K>::FindRoot()
{
    iterator iter_root = begin();
    if (iter_root.node_ != nullptr) {
        while (iter_root.node_->parent != nullptr) {
            ++iter_root;
        }
    }
    tree_node_ = iter_root.node_;
}

template<typename K>
void RBTree<K>::ConnectNodeParentWithPivot(TreeNode<value_type> *node, TreeNode<value_type> *pivot)
{
    pivot->parent = node->parent;
    if (node->parent != nullptr) { // node -> parent соединяется с pivot
        if (node->parent->left == node) {
            node->parent->left = pivot;
        } else {
            node->parent->right = pivot;
        }
    }
}

template<typename K>
void RBTree<K>::RotateLeft(TreeNode<value_type> *node)
{
    TreeNode<value_type> *pivot = node->right;

    ConnectNodeParentWithPivot(node, pivot);

    node->right = pivot->left;
    if (pivot->left != nullptr) {
        pivot->left->parent = node;
    }
    node->parent = pivot;
    pivot->left = node;
}

template<typename K>
void RBTree<K>::RotateRight(TreeNode<value_type> *node)
{
    TreeNode<value_type> *pivot = node->left;

    ConnectNodeParentWithPivot(node, pivot);

    node->left = pivot->right;
    if (pivot->right != nullptr) {
        pivot->right->parent = node;
    }
    node->parent = pivot;
    pivot->right = node;
}

template<typename K>
void RBTree<K>::InsertColoringCaseOne(iterator iter)
{
    if (iter.node_->parent == nullptr) { // Меняем цвет корня
        iter.node_->color = BLACK;
    } else if (iter.node_->parent->color != BLACK) {
        InsertColoringCaseTwo(iter); // Если два красных узла друг за другом
    }
}

template<typename K>
void RBTree<K>::InsertColoringCaseTwo(iterator &iter)
{
    iterator unc = Uncle(iter);
    if (unc.node_ != nullptr
        && unc.node_->color == RED) { // Если дядя красный, делаем прадеда красным,
        // дядю и родителя черными
        iterator grp = Grandparent(iter);
        grp.node_->color = RED;
        iter.node_->parent->color = BLACK;
        unc.node_->color = BLACK;
        InsertColoringCaseOne(grp);
    } else {
        InsertColoringCaseThree(iter); // если дяди нет или он черный
    }
}

template<typename K>
void RBTree<K>::InsertColoringCaseThree(iterator &iter)
{ // переносим на одну сторону красные
    iterator grp = Grandparent(iter);

    if (iter.node_ == iter.node_->parent->right
        && iter.node_->parent == grp.node_->left) { // на левую

        RotateLeft(iter.node_->parent);
        iter.node_ = iter.node_->left;
    } else if (iter.node_ == iter.node_->parent->left
               && iter.node_->parent == grp.node_->right) { // на правую

        RotateRight(iter.node_->parent);
        iter.node_ = iter.node_->right;
    }
    InsertColoringCaseFour(iter);
}

template<typename K>
void RBTree<K>::InsertColoringCaseFour(iterator &iter)
{
    iterator grp = Grandparent(iter);

    iter.node_->parent->color = BLACK;
    grp.node_->color = RED;
    if (iter.node_ == iter.node_->parent->left
        && iter.node_->parent == grp.node_->left) { // если два красных слева - поворачиваем направо
        RotateRight(grp.node_);
    } else { // аналогично налево
        RotateLeft(grp.node_);
    }
}

template<typename K>
void RBTree<K>::DeleteTwoChild(iterator &pos)
{
    iterator max_left(pos.node_->left);
    while (max_left.node_->right != nullptr) {
        max_left.node_ = max_left.node_->right;
    }
    if (max_left.node_->left != nullptr) {
        max_left.node_->left->parent = max_left.node_->parent;
    }
    if (max_left.node_ == max_left.node_->parent->left) {
        max_left.node_->parent->left = max_left.node_->left;
    } else {
        max_left.node_->parent->right = max_left.node_->left;
    }
    pos.node_->value = max_left.node_->value;
    delete max_left.node_;
    pos.node_ = nullptr;
    --number_of_elements_;
}

template<typename K>
void RBTree<K>::DeletingWithoutChild(iterator &pos)
{
    if (pos.node_->parent != nullptr) {
        if (pos.node_ == pos.node_->parent->left) {
            pos.node_->parent->left = nullptr;
        } else {
            pos.node_->parent->right = nullptr;
        }
    } else {
        tree_node_ = nullptr;
    }
    delete pos.node_;
    pos.node_ = nullptr;
    --number_of_elements_;
}

template<typename K>
void RBTree<K>::ReplaceNode(iterator &pos, iterator &child)
{
    child.node_->parent = pos.node_->parent;
    if (pos.node_->parent != nullptr) {
        if (pos.node_ == pos.node_->parent->left) {
            pos.node_->parent->left = child.node_;
        } else {
            pos.node_->parent->right = child.node_;
        }
    }
};

template<typename K>
void RBTree<K>::DeleteOneChild(iterator &pos)
{
    iterator child;
    if (pos.node_->right == nullptr) {
        child = pos.node_->left;
    } else {
        child = pos.node_->right;
    }
    ReplaceNode(pos, child);
    if (pos.node_->color == BLACK) {
        if (child.node_->color == RED) {
            child.node_->color = BLACK;
        } else {
            DeleteCaseOne(child);
        }
    }
    if (child.node_->parent != nullptr) {
        tree_node_ = child.node_->parent;
    } else {
        tree_node_ = child.node_;
    }
    FindRoot();

    delete pos.node_;
    pos.node_ = nullptr;
    --number_of_elements_;
}

template<typename K>
void RBTree<K>::DeleteCaseOne(iterator &iter)
{
    if (iter.node_->parent != nullptr) {
        iterator sib = Sibling(iter);
        if (sib.node_ == nullptr) {
            return;
        }
        if (sib.node_->color == RED) {
            iter.node_->parent->color = RED;
            sib.node_->color = BLACK;

            if (iter.node_ == iter.node_->parent->left) {
                RotateLeft(iter.node_->parent);
            } else {
                RotateRight(iter.node_->parent);
            }
        }
        DeleteCaseTwo(iter);
    }
}

template<typename K>
void RBTree<K>::DeleteCaseTwo(iterator &iter)
{
    iterator sib = Sibling(iter);
    if (sib.node_ == nullptr || sib.node_->left == nullptr || sib.node_->right == nullptr) {
        return;
    }
    if ((iter.node_->parent->color == BLACK) && (sib.node_->color == BLACK)
        && (sib.node_->left->color == BLACK) && (sib.node_->right->color == BLACK)) {
        sib.node_->color = RED;
        iter.node_ = iter.node_->parent;
        DeleteCaseOne(iter);
    } else {
        DeleteCaseThree(iter);
    }
}

template<typename K>
void RBTree<K>::DeleteCaseThree(iterator &iter)
{
    iterator sib = Sibling(iter);
    if ((iter.node_->parent->color == RED) && (sib.node_->color == BLACK)
        && (sib.node_->left->color == BLACK) && (sib.node_->right->color == BLACK)) {
        sib.node_->color = RED;
        iter.node_->parent->color = BLACK;
    } else {
        DeleteCaseFour(iter);
    }
}

template<typename K>
void RBTree<K>::DeleteCaseFour(iterator &iter)
{
    iterator sib = Sibling(iter);
    if (sib.node_->color == BLACK) {
        if ((iter.node_ == iter.node_->parent->left) && (sib.node_->right->color == BLACK)
            && (sib.node_->left->color == RED)) {
            sib.node_->color = RED;
            sib.node_->left->color = BLACK;
            RotateRight(sib.node_);
        } else if ((iter.node_ == iter.node_->parent->right) && (sib.node_->left->color == BLACK)
                   && (sib.node_->right->color == RED)) {
            sib.node_->color = RED;
            sib.node_->right->color = BLACK;
            RotateLeft(sib.node_);
        }
    }
    DeleteCaseFive(iter);
}

template<typename K>
void RBTree<K>::DeleteCaseFive(iterator &iter)
{
    iterator sib = Sibling(iter);

    sib.node_->color = iter.node_->parent->color;
    iter.node_->parent->color = BLACK;

    if (iter.node_ == iter.node_->parent->left) {
        sib.node_->right->color = BLACK;
        RotateLeft(iter.node_->parent);
    } else {
        sib.node_->left->color = BLACK;
        RotateRight(iter.node_->parent);
    }
}

template<typename K>
template<typename KT, typename T>
bool RBTree<K>::IsItMore(const std::pair<KT, T> &arg_one, const std::pair<KT, T> &arg_two) const
{
    return arg_one.first > arg_two.first;
}

template<typename K>
template<typename KT, typename T>
bool RBTree<K>::IsItLess(const std::pair<KT, T> &arg_one, const std::pair<KT, T> &arg_two) const
{
    return arg_one.first < arg_two.first;
}

template<typename K>
template<typename KT, typename T>
bool RBTree<K>::AreTheyEqual(const std::pair<KT, T> &arg_one, const std::pair<KT, T> &arg_two) const
{
    return arg_one.first == arg_two.first;
}

template<typename K>
template<typename KT>
bool RBTree<K>::IsItMore(const KT &arg_one, const KT &arg_two) const
{
    return arg_one > arg_two;
}

template<typename K>
template<typename KT>
bool RBTree<K>::IsItLess(const KT &arg_one, const KT &arg_two) const
{
    return arg_one < arg_two;
}

template<typename K>
template<typename KT>
bool RBTree<K>::AreTheyEqual(const KT &arg_one, const KT &arg_two) const
{
    return arg_one == arg_two;
}

// *-------------------- CLASS ITERATOR  --------------------*

template<typename K>
typename RBTree<K>::TreeIterator::value_type RBTree<K>::TreeIterator::operator*()
{
    if (node_ == nullptr) {
        static value_type defaultValue = value_type{};
        return defaultValue;
    }
    return node_->value;
}

template<typename K>
typename RBTree<K>::TreeIterator::TreeIterator RBTree<K>::TreeIterator::operator++(int)
{
    TreeIterator temp(*this);
    operator++();
    return temp;
}

template<typename K>
typename RBTree<K>::TreeIterator::TreeIterator &RBTree<K>::TreeIterator::operator++()
{
    if (node_ != nullptr) {
        SearchForMore();
    } else if (pr_node_ != nullptr) {
        node_ = pr_node_;
        pr_node_ = nullptr;
    }
    return *this;
}

template<typename K>
typename RBTree<K>::TreeIterator::TreeIterator RBTree<K>::TreeIterator::operator--(int)
{
    TreeIterator temp(*this);
    operator--();
    return temp;
}

template<typename K>
typename RBTree<K>::TreeIterator::TreeIterator &RBTree<K>::TreeIterator::operator--()
{
    if (node_ != nullptr) {
        SearchForLess();
    } else if (pr_node_ != nullptr) {
        node_ = pr_node_;
        pr_node_ = nullptr;
    }
    return *this;
}

template<typename K>
typename RBTree<K>::TreeIterator::TreeIterator &RBTree<K>::TreeIterator::operator=(
    TreeIterator &&other)
{
    if (this != &other) {
        node_ = other.node_;
        pr_node_ = other.pr_node_;
        other.node_ = nullptr;
        other.pr_node_ = nullptr;
    }
    return *this;
}

template<typename K>
typename RBTree<K>::TreeIterator::TreeIterator &RBTree<K>::TreeIterator::operator=(
    TreeNode<value_type> *other)
{
    if (node_ != other) {
        node_ = other;
    }
    return *this;
}

template<typename K>
bool RBTree<K>::TreeIterator::operator==(const TreeIterator &other) const
{
    return node_ == other.node_;
}

template<typename K>
bool RBTree<K>::TreeIterator::operator!=(const TreeIterator &other) const
{
    return node_ != other.node_;
};

template<typename K>
void RBTree<K>::TreeIterator::SearchForMore()
{
    if (node_->right == nullptr) {
        pr_node_ = node_;
        node_ = FindParent(node_, true);
    } else {
        node_ = node_->right;
        while (node_->left != nullptr) {
            node_ = node_->left;
        }
    }
}

template<typename K>
void RBTree<K>::TreeIterator::SearchForLess()
{
    if (node_->left == nullptr) {
        pr_node_ = node_;
        node_ = FindParent(node_, false);
    } else {
        node_ = node_->left;
        while (node_->right != nullptr) {
            node_ = node_->right;
        }
    }
}

template<typename K>
TreeNode<typename RBTree<K>::TreeIterator::value_type> *RBTree<K>::TreeIterator::FindParent(
    TreeNode<value_type> *temp_node, bool move_right)
{
    TreeNode<value_type> *parent = temp_node->parent;
    if (temp_node->parent != nullptr) {
        while (parent != nullptr && temp_node != (move_right ? parent->left : parent->right)) {
            temp_node = parent;
            parent = temp_node->parent;
        }
    }
    return parent;
}
