#include "BTreeNode.hpp"
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
BTreeNode<T> *BTreeNode<T>::split(T key)
{
    BTreeNode<T> *newNode = new BTreeNode<T>(this->M, isLeaf);

    int i;
    for (i = 0; i < (M - 2) / 2; i++)
    {
        newNode->keys[i] = keys[i + (M - 1) / 2 + 1];
    }
    // guarda no novo node o overflow que esta guardado em key
    newNode->keys[i] = key;
    newNode->currKeys = M / 2;

    currKeys /= 2;

    // Se nao for folha, partilha ponteiros para filhos
    if (!isLeaf)
    {
        for (i = 0; i < M / 2; i++)
        {
            newNode->children[i] = children[i + M / 2 + 1];
        }
    }

    return newNode;
}

template <class T>
BTreeNode<T> *BTreeNode<T>::insertLocal(T key)
{
    T aux;
    int i;

    i = 0;
    while (i < currKeys && key > keys[i])
        i++;

    // Insere a chave e desloca para a direita
    // Caso haja overflow, ele estara guardado na variavel key
    while (i <= currKeys && i < M - 1)
    {
        aux = keys[i];
        keys[i] = key;
        key = aux;
        i++;
    }

    // Se estiver cheio, um overflow ocorreu
    if (currKeys == M - 1)
    {
        // Particiona este node e guarde a referencia para o novo node criado
        // Passa o overflow como parametro
        BTreeNode *newNode = split(key);

        // Retorna ponteiro para novo node
        return newNode;

        // Nao eh preciso retornar a key do meio pois ela ainda esta salva no vetor de keys
        // E o node pai sabe onde acessar
    }
    else
    {
        currKeys++;
        return nullptr;
    }
}

template <class T>
BTreeNode<T> *BTreeNode<T>::insert(T key)
{
    BTreeNode *newChild;

    // Se eh uma folha, nao eh preciso verificar filhos
    if (isLeaf)
    {
        newChild = insertLocal(key);

        return newChild;
    }

    // Se nao eh uma folha, ache o index e insira a chave no filho
    int i = 0;
    while (i < currKeys && key > keys[i])
        i++;

    // Se houver um overflow para baixo, uma referencia para um novo node sera retorarnada
    newChild = children[i]->insert(key);

    // Se ha um novo node para ser adcionado a lista
    if (newChild != nullptr)
    {
        // Tambem ha no filho inserido uma chave para ser passada para cima
        T newKey = children[i]->keys[(M - 1) / 2];

        BTreeNode<T> *aux;

        // Se este node tambem esta cheio
        if (currKeys == M - 1)
        {

            BTreeNode<T> *newNode = insertLocal(newKey);

            // Insere a nova chave no node partido
            // Como houve a divisao e dois ponteiros ficam no primeiro node, diminui o index em 2
            for (int j = i - 2; j <= newNode->currKeys; j++)
            {
                aux = newNode->children[j];
                newNode->children[j] = newChild;
                newChild = aux;
            }

            return newNode;
        }
        else // Se nao estiver cheio, apenas insira a chave e o novo filho
        {
            insertLocal(newKey);

            // insere novo filho e move para direita
            for (int j = i + 1; i < currKeys; i++)
            {
                aux = children[j];
                children[j] = newChild;
                newChild = aux;
            }
        }
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
    for (int i = 0; i < currKeys; i++)
        std::cout << keys[i] << " - ";

    std::cout << std::endl;

    // // There are n keys and n+1 children, traverse through n keys
    // // and first n children
    // int i;
    // for (i = 0; i < currKeys; i++)
    // {
    //     // If this is not leaf, then before printing key[i],
    //     // traverse the subtree rooted with child C[i].
    //     if (isLeaf == false)
    //         children[i]->show();
    //     std::cout << " " << keys[i];
    // }

    // // Print the subtree rooted with last child
    // if (isLeaf == false)
    //     children[i]->show();
}

template class BTreeNode<int>;