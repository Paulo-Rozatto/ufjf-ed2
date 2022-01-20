#ifndef B_TREE_HPP
#define B_TREE_HPP

#include "BTreeNode.hpp"

template <class T>
class BTree
{
private:
    int M;
    BTreeNode<T> *root;

public:
    BTree(int M);
    ~BTree();
    void insert(T key, int *cont);
    BTreeNode<T> *search(T key, int *cont) { return root->search(key, cont); };
};

#endif