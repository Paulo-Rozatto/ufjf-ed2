#ifndef B_TREE_HPP
#define B_TREE_HPP

#include "BTreeNode.hpp"

template <class T, int M>
class BTree
{
private:
public:
    BTreeNode<T> *root;
    BTree(/* args */);
    ~BTree(){};
    void insert(T key);
};


#endif