#include "BTree.hpp"
#include "BKey.hpp"
#include <iostream>

template <class T>
BTree<T>::BTree(int M)
{
    root = nullptr;
    this->M = M;
}

template <class T>
BTree<T>::~BTree()
{
    delete root;
}

template <class T>
void BTree<T>::insert(T key, int *cont)
{
    if (root == nullptr)
    {
        root = new BTreeNode<T>(M, true);
        root->keys[0] = key;
        root->currKeys = 1;
    }
    else if (root->currKeys == M - 1)
    {
        BTreeNode<T> *newRoot = new BTreeNode<T>(M, false);

        newRoot->children[0] = root;

        newRoot->split(0, root);

        // newRoot->insert(key);

        root = newRoot;

        if (root->keys[0] < key)
        {
            root->children[1]->insert(key, cont);
            *cont += 1;
        }
        else
        {
            root->children[0]->insert(key, cont);
            *cont += 1;
        }
    }
    else
    {
        root->insert(key, cont);
    }
}

template class BTree<BKey>;
// template class BTree<int>;
