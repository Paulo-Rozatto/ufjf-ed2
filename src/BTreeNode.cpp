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
    // A metade do valor maximo e utitlizada varias vezes abaixo
    // Entao ela foi guardada em uma variavel auxiliar
    int md = M / 2;

    // A divisao ira gerar um novo node com metade - 1 chaves
    BTreeNode *newChild = new BTreeNode(child->M, child->isLeaf);
    newChild->currKeys = md - 1;

    // O novo no ira receber a sedunda metade de chaves do filho atual
    for (int j = 0; j < md - 1; j++)
        newChild->keys[j] = child->keys[j + md];

    // Se o filho atual for chave, tem-se que dividir os filhos tambem
    if (child->isLeaf == false)
    {
        for (int j = 0; j < md; j++)
            newChild->children[j] = child->children[j + md];
    }

    // O filho antigo tambem tera metade das chaves apos a divisao
    child->currKeys = md - 1;

    /// Precisa-entao abrir espaco no node pai para guardar o novo filho
    // E entao inseri-lo
    for (int j = currKeys; j >= i + 1; j--)
        children[j + 1] = children[j];
    children[i + 1] = newChild;

    // Uma chave tambem ira subir para o node pai, entao precisa-se tambem de abrin
    // espaco e inserir a nova chave
    for (int j = currKeys - 1; j >= i; j--)
        keys[j + 1] = keys[j];
    keys[i] = child->keys[md - 1];

   // O numero de chaves no node pai apos a divisao aumenta
    currKeys = currKeys + 1;

    return nullptr;
}

template <class T>
BTreeNode<T> *BTreeNode<T>::insert(T key, int *cont)
{
   // i recebe o indice do valor mais a direita
    int i = currKeys - 1;

    if (isLeaf == true)
    {
        // The following loop does two things
        // a) Finds the location of new key to be inserted
        // b) Moves all greater keys to one place ahead

        // Pode-se abrir espaco para insercao ao mesmo tempo que
        // Se procura onde colocar a nova chave
        while (i >= 0 && keys[i] > key)
        {
            keys[i + 1] = keys[i];
            i--;
            *cont += 1;
        }
        *cont += 1;

        keys[i + 1] = key;
        currKeys = currKeys + 1;
    }
    else 
    {
        while (i >= 0 && keys[i] > key)
        {
            i--;
            *cont += 1;
        }
        *cont += 1;

        // Se o filho para inservao estiver lotado, divida de uma vez
        if (children[i + 1]->currKeys == M - 1)
        {
            split(i + 1, children[i + 1]);

            // After split, the middle key of children[i] goes up and
            // children[i] is splitted into two. See which of the two
            // is going to have the new key

            // Se a chave na posicao i e menor que a chave de insercao
            // Significa que tem que inserir essa chave no proximo node
            // O que implica no novo node filho criado.
            if (keys[i + 1] < key)
            {
                *cont += 1;
                i++;
            }
        }
        children[i + 1]->insert(key, cont);
    }

    return nullptr;
}

template <class T>
BTreeNode<T> *BTreeNode<T>::search(T key, int *cont)
{
    int i = 0;
    while (i < currKeys && key > keys[i])
    {
        i++;
        *cont += 1;
    }
    *cont += 1;

    *cont += 1;
    if (keys[i] == key)
        return this;

    if (isLeaf)
        return nullptr;

    return children[i]->search(key, cont);
    return nullptr;
}
template class BTreeNode<BKey>;