#include "BinaryTreeSearch.h"

#include <iostream>

template<typename T>
BinaryTreeSearch<T>::BinaryTreeSearch(T value)
    : _root(new Node(value))
    , _size(1)
{}

template<typename T>
BinaryTreeSearch<T>::~BinaryTreeSearch() = default;

template<typename T>
size_t BinaryTreeSearch<T>::size() const
{
    return _size;
}

template<typename T>
typename BinaryTreeSearch<T>::Node *BinaryTreeSearch<T>::add(Node *root, T value)
{
    if (root == nullptr) {
        ++_size;
        return new Node(value);
    }

    (value < root->value) ? root->left.reset(add(root->left.get(), value))
                          : root->right.reset(add(root->right.get(), value));

    return root;
}

template<typename T>
typename BinaryTreeSearch<T>::Node *BinaryTreeSearch<T>::findMin(Node *root) const
{
    return (root->left.get()) ? findMin(root->left.get()) : root;
}

template<typename T>
typename BinaryTreeSearch<T>::Node *BinaryTreeSearch<T>::find(Node *root, T value) const
{
    if (root->value == value || root == nullptr) {
        return root;
    }
    return (value < root->value) ? find(root->left.get(), value) : find(root->right.get(), value);
}

template<typename T>
typename BinaryTreeSearch<T>::Node *BinaryTreeSearch<T>::remove(Node *root, T value)
{
    if (value < root->value) {
        root->left.reset(remove(root->left.get(), value));
    } else if (value > root->value) {
        root->right.reset(remove(root->right.get(), value));
    } else {
        if (!root->left) {
            return root->right.release();
        }
        if (!root->right) {
            return root->left.release();
        }

        Node *minNode = findMin(root->right.get());
        root->value = minNode->value;
        root->right.reset(remove(root->right.get(), minNode->value));
    }
    return root;
}

template<typename T>
typename BinaryTreeSearch<T>::Node *BinaryTreeSearch<T>::getRoot()
{
    return _root.get();
}

template<typename T>
void BinaryTreeSearch<T>::traverse(Node *node) const
{
    if (node) {
        traverse(node->left.get());
        std::cout << node->value << ' ';
        traverse(node->right.get());
    }
}

template<typename T>
typename BinaryTreeSearch<T>::Node *BinaryTreeSearch<T>::insertSubtree(Node *node,
                                                                       std::unique_ptr<Node> subtree)
{
    if (node == nullptr) {
        return subtree.release();
    }
    if (subtree->value < node->value) {
        node->left.reset(insertSubtree(node->left.get(), std::move(subtree)));
    } else {
        node->right.reset(insertSubtree(node->right.get(), std::move(subtree)));
    }
    return node;
}

template<typename T>
typename std::unique_ptr<typename BinaryTreeSearch<T>::Node> BinaryTreeSearch<T>::removeSubtree(
    Node *node, T value)
{
    if (node == nullptr) {
        return nullptr;
    }

    if (value < node->value) {
        node->left = removeSubtree(node->left.get(), value);
    } else if (value > node->value) {
        node->right = removeSubtree(node->right.get(), value);
    } else {
        return std::unique_ptr<Node>(node);
    }
    return nullptr;
}

template<typename T>
typename BinaryTreeSearch<T>::Node *BinaryTreeSearch<T>::lowestCommonAncestor(Node *node,
                                                                              T value1,
                                                                              T value2) const
{
    if (node == nullptr) {
        return nullptr;
    }
    if (node->value > value1 && node->value > value2) {
        return lowestCommonAncestor(node->left.get(), value1, value2);
    }
    if (node->value < value1 && node->value < value2) {
        return lowestCommonAncestor(node->right.get(), value1, value2);
    }
    return node;
}

template class BinaryTreeSearch<int32_t>;
