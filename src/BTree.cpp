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
            // BTreeNode<T> *test = root->children[1];
            // return;
        }

        // Se raiz está cheia
        // if (root->currKeys == (M - 1))
        // {
        //     BTreeNode<T> *newRoot = new BTreeNode<T>(M, false);
        //     BTreeNode<T> *secondChild;
        //     newRoot->children[0] = root;
        //     root = newRoot;

        //     secondChild = root->insert(key);

        //     if (secondChild != nullptr)
        //     {
        //         std::cout << "Aui" << std::endl;
        //     }
        //     // newRoot->children[1] = secondChild;
        // }
        // else
        // {
        //     root->insert(key);
        // }
    }
}

// template <class T, int M>
// void BTree<T, M>::insert(T key)
// {
//     if (root == nullptr)
//     {
//         root = new BTreeNode<T>(M, true);
//         root->insert(key);
//         return;
//     }
//     else
//     {
//         bool isLeaf = false;
//         int i, nodeIndex = 0;
//         BTreeNode<T> *node = root, *parent = nullptr;

//         // encontra folha
//         while (true)
//         {
//             i = 0;
//             while (i < node->currKeys && key > node->keys[i])
//                 i++;

//             if (node->isLeaf)
//                 break;
//             else
//             {
//                 parent = node;
//                 node = node->children[i];
//                 nodeIndex = i;
//             }
//         }

//         // Insira chave no local e mova a direita outros valores
//         T aux;
//         while (i < node->currKeys)
//         {
//             aux = node->keys[i];
//             node->keys[i] = key;
//             key = aux;
//             i++;
//         }
//         // Caso o node esteja cheio, o overflow vai ficar na variavel key

//         // Caso esteja cheio
//         // while (node->currKeys == node->M - 1)
//         // {

//         // }

//         // Se nao esiver cheio
//         // if (node->currKeys < node->M - 1)
//         // {
//         // // Insira chave no local e mova a direita outros valores
//         // T aux;
//         // while (i < node->currKeys)
//         // {
//         //     aux = node->keys[i];
//         //     node->keys[i] = key;
//         //     key = aux;
//         //     i++;
//         // }
//     }

//     // i = 1;
//     // while (i < node->currKeys && key > node->keys[i])
//     // i++;
// }

template class BTree<int, 5>;