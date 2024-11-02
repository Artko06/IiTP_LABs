#ifndef BINARYTREESEARCH_H
#define BINARYTREESEARCH_H

#include <memory>

template<typename T>
class BinaryTreeSearch
{
private:
    struct Node
    {
        T value;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        explicit Node(T value)
            : value(value)
            , left(nullptr)
            , right(nullptr)
        {}
    };

    std::unique_ptr<Node> _root;
    size_t _size;

public:
    explicit BinaryTreeSearch(T value);
    ~BinaryTreeSearch();

    size_t size() const;

    Node *add(Node *node, T value);
    Node *findMin(Node *root) const;
    Node *find(Node *root, T value) const;
    Node *remove(Node *root, T value);
    Node *getRoot();
    void traverse(Node *node) const;

    Node *insertSubtree(Node *node, std::unique_ptr<Node> subtree);
    std::unique_ptr<Node> removeSubtree(Node *node, T value);
    Node *lowestCommonAncestor(Node *node, T value1, T value2) const;
};

#endif // BINARYTREESEARCH_H
