#include "BTree.hpp"
#include <iostream>

template <class T, int M>
BTree<T, M>::BTree()
{
    root = nullptr;
}

template <class T, int M>
void BTree<T, M>::insert(T key)
{
    if (root == nullptr)
    {
        root = new BTreeNode<T>(M, true);
        root->insert(key);
        return;
    }
    else
    {
        BTreeNode<T> *newChild = root->insert(key);

        if (newChild != nullptr)
        {
            BTreeNode<T> *newRoot = new BTreeNode<T>(M, false);

            newRoot->insertLocal(root->keys[(M - 1) / 2]);
            newRoot->children[0] = root;
            newRoot->children[1] = newChild;
            newRoot->currKeys = 1;

            root = newRoot;
        }

    }
}

template class BTree<int, 5>;