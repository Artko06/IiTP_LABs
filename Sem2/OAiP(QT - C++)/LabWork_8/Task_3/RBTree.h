#ifndef RBTREE_H
#define RBTREE_H

#include <QString>
#include <cstddef>
#include <utility>

enum node_color { RED, BLACK };

template<typename T>
struct TreeNode
{
    using value_type = T;

    TreeNode *parent, *left, *right;
    node_color color;
    value_type value;
};

template<typename K>
class RBTree
{
public:
    class TreeIterator
    {
    public:
        using value_type = K;

        TreeIterator() = default;
        explicit TreeIterator(TreeNode<value_type> *other_node)
            : node_(other_node){};
        TreeIterator(const TreeIterator &other)
            : node_(other.node_){};
        TreeIterator(TreeIterator &&other) noexcept
            : node_(other.node_){};
        ~TreeIterator() = default;

        value_type operator*();
        TreeIterator &operator=(TreeIterator &&other);
        TreeIterator &operator=(TreeNode<value_type> *other);
        TreeIterator operator++(int); // Оператор постфикстный инкремент
        TreeIterator &operator++();   // Оператор префиксный инкемент
        TreeIterator operator--(int);
        TreeIterator &operator--();
        bool operator==(const TreeIterator &other) const;
        bool operator!=(const TreeIterator &other) const;

    private:
        TreeNode<value_type> *node_ = nullptr;
        TreeNode<value_type> *pr_node_ = nullptr; // Предыдущий узел
        friend class RBTree;

        void SearchForMore(); // Поикс большего чем этот узел
        void SearchForLess(); // Поиск меньшего чем этот узел

        // Поиск нужного родителя и для оператора ++ и для --
        TreeNode<value_type> *FindParent(TreeNode<value_type> *temp_node, bool move_right);
    };

    class TreeConstIterator : public RBTree<K>::TreeIterator
    {
    public:
        using TreeIterator::TreeIterator;
        using typename TreeIterator::value_type;
        using const_reference = const value_type &;

        explicit TreeConstIterator(const TreeIterator &other)
            : TreeIterator(other)
        {}

        const_reference operator*() const
        {
            if (TreeIterator::node_ == nullptr) {
                static value_type defaultValue = value_type{};
                return defaultValue;
            }
            return TreeIterator::node_->value;
        };
    };

    using value_type = K;
    using reference = value_type &;
    using const_reference = const value_type &;
    using iterator = TreeIterator;
    using const_iterator = TreeConstIterator;
    using size_type = size_t;

    RBTree() = default;
    RBTree(const RBTree &other);
    RBTree(RBTree &&other) noexcept;
    ~RBTree();

    iterator begin() const;
    iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    bool empty() const;

    size_type size() const;

    void swap(RBTree &other);
    void clear();
    void erase(iterator pos);
    void merge(RBTree &other);

protected:
    // возрат содержимого, если есть
    std::pair<TreeNode<value_type> *, bool> content(const value_type &value) const;
    std::pair<iterator, bool> insertInTree(const value_type &value, std::pair<bool, bool> mode);
    // проверка на принадлежность к этому классу
    bool equalityOfClasses(const RBTree &other);

private:
    // Корень дерева
    TreeNode<value_type> *tree_node_ = nullptr;
    size_type number_of_elements_ = 0;

    void ClearRecursive(TreeNode<value_type> *&node_) const;

    std::pair<iterator, bool> InsertOtherNodes(const value_type &value, std::pair<bool, bool> mode);

    iterator InsertEquals(TreeNode<value_type> *node_, const value_type &value);

    TreeNode<value_type> *CreateNode(const value_type &value); // сорздание узла

    iterator CreatingANode(TreeNode<value_type> *node_,
                           const value_type &value,
                           bool right); // создание узла с итератором
    iterator Grandparent(iterator &iter);
    iterator Sibling(iterator &iter);
    iterator Uncle(iterator &iter);
    void FindRoot();
    void ConnectNodeParentWithPivot(TreeNode<value_type> *node, TreeNode<value_type> *pivot);
    void RotateLeft(TreeNode<value_type> *node_);
    void RotateRight(TreeNode<value_type> *node_);
    void InsertColoringCaseOne(iterator iter);
    void InsertColoringCaseTwo(iterator &iter);
    void InsertColoringCaseThree(iterator &iter);
    void InsertColoringCaseFour(iterator &iter);
    void DeleteTwoChild(iterator &pos);
    void DeletingWithoutChild(iterator &pos);
    void ReplaceNode(iterator &pos, iterator &child);
    void DeleteOneChild(iterator &pos);
    void DeleteCaseOne(iterator &iter);
    void DeleteCaseTwo(iterator &iter);
    void DeleteCaseThree(iterator &iter);
    void DeleteCaseFour(iterator &iter);
    void DeleteCaseFive(iterator &iter);

    template<typename KT, typename T>
    bool IsItMore(const std::pair<KT, T> &arg_one, const std::pair<KT, T> &arg_two) const;

    template<typename KT, typename T>
    bool IsItLess(const std::pair<KT, T> &arg_one, const std::pair<KT, T> &arg_two) const;

    template<typename KT, typename T>
    bool AreTheyEqual(const std::pair<KT, T> &arg_one, const std::pair<KT, T> &arg_two) const;

    template<typename KT>
    bool IsItMore(const KT &arg_one, const KT &arg_two) const;

    template<typename KT>
    bool IsItLess(const KT &arg_one, const KT &arg_two) const;

    template<typename KT>
    bool AreTheyEqual(const KT &arg_one, const KT &arg_two) const;
};

template class RBTree<std::pair<int32_t, QString>>;

#endif // RBTREE_H
