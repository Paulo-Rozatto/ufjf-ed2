#include "BTreeNode.hpp"
#include "BKey.hpp"
#include <iostream>

template <class T>
BTreeNode<T>::BTreeNode(int M, bool isLeaf)
{
    this->M = M;
    this->isLeaf = isLeaf;

    currKeys = 0;
    keys = new T[M - 1];
    children = new BTreeNode *[M];
}

template <class T>
BTreeNode<T>::~BTreeNode()
{
    if (!isLeaf)
    {
        for (int i = 0; i <= currKeys; i++)
        {
            delete children[i];
        }
    }
    delete[] keys;
    delete[] children;
}

template <class T>
BTreeNode<T> *BTreeNode<T>::split(int i, BTreeNode *child)
{
    // // A metade do tamanho maximo precisa ser usado varias vezes, entao salva numa variavel
    // int half = M / 2 + 1;

    // // Cria novo node
    // BTreeNode<T> *newChild = new BTreeNode<T>(child->M, child->isLeaf);

    // // O novo node tera metada da capacidade - 1 chaves
    // newChild->currKeys = half - 1;

    // for (int j = 0; j < half - 1; j++)
    // {
    //     newChild->keys[j] = child->keys[j + half];
    // }

    // if (!child->isLeaf)
    // {
    //     for (int j = 0; j < half; j++)
    //     {
    //         newChild->children[j] = child->children[j + half];
    //     }
    // }

    // child->currKeys = half - 1;

    // // abre espaco a direita de child para abrigar o newChild
    // for (int j = currKeys; j >= i + 1; j--)
    // {
    //     this->children[currKeys + 1] = this->children[currKeys];
    // }

    // // insere newChild
    // this->children[i + 1] = newChild;

    // // Precisa-se entao inserir a key mediana de child
    // // Pela implementacao do insert, a mediana the child vai ser menor que a key na posicao i e maior que da i - 1
    // // Entao mova todas keys para direita a partir de i
    // for (int j = currKeys - 1; j >= i; j--)
    //     this->keys[j + 1] = this->keys[j];

    // // Insira a mediana de child na posicao i
    // this->keys[i] = child->keys[half - 1];

    // // incrementa o contador de chaves
    // this->currKeys++;

    // Create a new node which is going to store (md-1) keys
    // of child
    int md = M / 2;

    BTreeNode *newChild = new BTreeNode(child->M, child->isLeaf);
    newChild->currKeys = md - 1;

    // Copy the last (md-1) keys of child to newChild
    for (int j = 0; j < md - 1; j++)
        newChild->keys[j] = child->keys[j + md];

    // Copy the last md children of child to newChild
    if (child->isLeaf == false)
    {
        for (int j = 0; j < md; j++)
            newChild->children[j] = child->children[j + md];
    }

    // Reduce the number of keys in child
    child->currKeys = md - 1;

    // Since this node is going to have a new child,
    // create space of new child
    for (int j = currKeys; j >= i + 1; j--)
        children[j + 1] = children[j];

    // Link the new child to this node
    children[i + 1] = newChild;

    // A key of child will move to this node. Find the location of
    // new key and move all greater keys one space ahead
    for (int j = currKeys - 1; j >= i; j--)
        keys[j + 1] = keys[j];

    // Copy the middle key of child to this node
    keys[i] = child->keys[md - 1];

    // Increment count of keys in this node
    currKeys = currKeys + 1;

    return nullptr;
}

template <class T>
BTreeNode<T> *BTreeNode<T>::insert(T key)
{
    // int i = currKeys - 1;

    // if (this->isLeaf)
    // {
    //     // Encontre a posicao onde inserir a chave
    //     while (i >= 0 && this->keys[i] > key)
    //     {
    //         this->keys[i + 1] = this->keys[i];
    //         i--;
    //     }

    //     // Abre espaco para a nova chave
    //     // for (int j = currKeys - 1; j >= i; j--)
    //     //     this->keys[j + 1] = this->keys[j];

    //     keys[i + 1] = key;
    //     currKeys++;
    //     return nullptr;
    // }

    // // Encontre a posicao onde inserir a chave
    // while (i >= 0 && this->keys[i] > key)
    // {
    //     i--;
    // }

    // // Se o filho dessa posicao estiver cheio, divida-o
    // if (this->children[i + 1]->currKeys == M - 1)
    // {
    //     split(i + 1, this->children[i + 1]);

    //     // Já que o node filho foi divido em dois, precisa-se determinar em qual colocar a chave
    //     // Se a chave que subiu do filho for menor que o valor a inserir, entao se insere no novo criado a direita
    //     // Do contrario, insira no antigo mesmo
    //     if (this->keys[i + 1] < key)
    //         i++;
    // }
    // this->children[i + 1]->insert(key);

    // Initialize index as index of rightmost element
    int i = currKeys - 1;

    // If this is a leaf node
    if (isLeaf == true)
    {
        // The following loop does two things
        // a) Finds the location of new key to be inserted
        // b) Moves all greater keys to one place ahead
        while (i >= 0 && keys[i] > key)
        {
            keys[i + 1] = keys[i];
            i--;
        }

        // Insert the new key at found location
        keys[i + 1] = key;
        currKeys = currKeys + 1;
    }
    else // If this node is not leaf
    {
        // Find the child which is going to have the new key
        while (i >= 0 && keys[i] > key)
            i--;

        // See if the found child is full
        if (children[i + 1]->currKeys == M - 1)
        {
            // If the child is full, then split it
            split(i + 1, children[i + 1]);

            // After split, the middle key of children[i] goes up and
            // children[i] is splitted into two. See which of the two
            // is going to have the new key
            if (keys[i + 1] < key)
                i++;
        }
        children[i + 1]->insert(key);
    }

    return nullptr;
}

template <class T>
BTreeNode<T> *BTreeNode<T>::search(T key)
{
    int i = 0;
    while (i < currKeys && key > keys[i])
    {
        i++;
    }

    if (keys[i] == key)
        return this;

    if (isLeaf)
        return nullptr;

    return children[i]->search(key);
}

template <class T>
void BTreeNode<T>::show()
{
    // for (int i = 0; i < currKeys; i++)
    //     std::cout << keys[i] << " - ";

    // std::cout << std::endl;

    // There are n keys and n+1 children, traverse through n keys
    // and first n children
    int i;
    for (i = 0; i < currKeys; i++)
    {
        // If this is not leaf, then before printing key[i],
        // traverse the subtree rooted with child C[i].
        if (isLeaf == false)
            children[i]->show();
        std::cout << " " << keys[i];
    }

    // Print the subtree rooted with last child
    if (isLeaf == false)
        children[i]->show();
}

template class BTreeNode<BKey>;
// template class BTreeNode<int>;