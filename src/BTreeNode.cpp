#include "BTreeNode.hpp"
#include <iostream>

template <class T>
BTreeNode<T>::BTreeNode(int M)
{
    this->M = M;
    isLeaf = false;
    currKeys = 0;

    keys = new T[M - 1];
    children = new BTreeNode *[M];
}

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
void BTreeNode<T>::splitChild(int i, BTreeNode *c)
{
    BTreeNode *nw = new BTreeNode<T>(M, true);

    // Copia metade dos valores para a nova folha
    for (int j = c->currKeys / 2; j < c->currKeys; j++)
    {
        nw->insert(keys[j]);
    }

    // Copia metade filhos para nova folha
    if (!c->isLeaf)
    {
        for (int j = c->currKeys / 2; j < c->currKeys + 1; j++)
        {
            nw->children[j] = c->children[j];
        }
    }

    // reduz o contador da folha atual
    c->currKeys /= 2;

    // c->show();
    // nw->show();

    // Desloca os ponteiros dos filhos
    BTreeNode *aux;
    for (int j = i + 1; j <= currKeys; j++)
    {
        aux = children[j];
        children[j] = nw;
        nw = aux;
    }
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
            newNode->children[i] = children[i + M / 2];
        }
    }

    return newNode;
}

template <class T>
BTreeNode<T> *BTreeNode<T>::insertLocal(T key)
{
    T aux;
    int i;

    // T h1[] = {keys[0], keys[1], keys[2], keys[3]};
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

    // T h2[] = {keys[0], keys[1], keys[2], keys[3]};

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

            for (int j = 0; i <= newNode->currKeys; i++)
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

// template <class T>
// void BTreeNode<T>::insert(T key)
// {
//     int i = 0;

//     // procura chave para deslocar
//     while (i < currKeys && key > keys[i])
//         i++;

//     if (isLeaf)
//     {
//         // desloca para direita chaves e abre espaco na posicao i
//         for (int j = currKeys - 1; j >= i; j--)
//             keys[j + 1] = keys[j];

//         // insere na posicao i
//         keys[i] = key;
//         currKeys++;
//     }
//     else // senao e folha, insere no filho
//     {
//         // se filho nao estiver cheio
//         if (children[i]->currKeys < M - 1)
//         {
//             children[i]->insert(key);
//         }
//         else // se estiver cheio, divida
//         {
//             BTreeNode *neo = new BTreeNode<T>(M, children[i]->isLeaf);
//             T aux;
//             int j;

//             // insere e desloca para a direita, o overflow vai ficar na variavel key
//             for (j = 0; j < M - 1; j++)
//             {
//                 if (key < children[i]->keys[j])
//                 {
//                     aux = children[i]->keys[j];
//                     children[i]->keys[j] = key;
//                     key = aux;
//                 }
//             }

//             // insere metade dos valores para novo node
//             for (j = 0; j < (M - 1) / 2; j++)
//             {
//                 neo->keys[j] = children[i]->keys[j + (M - 1) / 2 + 1];
//             }
//             neo->keys[j] = key; // coloca overflow no novo node
//             neo->currKeys = (M - 1) / 2 + 1;

//             if (!children[i]->isLeaf)
//             {
//                 // insere metade dos filhos para novo node
//                 for (j = 0; j < (M - 1) / 2; j++)
//                 {
//                     neo->children[j] = children[i]->children[j + (M - 1) / 2];
//                 }
//             }

//             children[i]->currKeys /= 2;

//             // insere valor do meio no node pai
//             key = children[i]->keys[(M - 1) / 2];
//             for (int j = currKeys - 1; j >= i; j--)
//                 keys[j + 1] = keys[j];
//             keys[i] = key;

//             // desloca para direita chaves e abre espaco na posicao i
//             // for (int j = M - 3; j >= i; j--)
//             //     children[i]->keys[j + 1] = children[i]->keys[j];

//             // insere na posicao i
//             // keys[i] = key;
//         }
//     }
// }

// template <class T>
// void BTreeNode<T>::insert(T key)
// {
//     int i = 0;
//     T aux;

//     // encontra posição para inserir chave
//     while (i < currKeys && key > keys[i])
//         i++;

//     if (isLeaf)
//     {
//         // incrementa contador de chaves
//         currKeys++;

//         // Insere chave e desloca outras chaves
//         while (i < currKeys)
//         {
//             aux = keys[i];
//             keys[i] = key;
//             key = aux;
//             i++;
//         }
//     }
//     else
//     {
//         // if (this->children[i]->currKeys == M - 1) // Filho cheio
//         // splitChild(i, children[i]);

//         BTreeNode *nw = new BTreeNode<T>(M, true),
//                   *mid = children[i];
//         T overKey;

//         // Copia metade dos valores para a nova folha
//         for (int j = c->currKeys / 2; j < mid->currKeys; j++)
//             nw->insert(keys[j]);

//         // Copia metade filhos para nova folha
//         if (!mid->isLeaf)
//         {
//             for (int j = mid->currKeys / 2; j < mid->currKeys + 1; j++)
//                 nw->children[j] = mid->children[j];
//         }

//         // reduz o contador da folha atual
//         mid->currKeys /= 2;

//         // mid->show();
//         // nw->show();

//         // Desloca os ponteiros dos filhos
//         BTreeNode *aux;
//         for (int j = i + 1; j <= currKeys; j++)
//         {
//             aux = children[j];
//             children[j] = nw;
//             nw = aux;
//         }
//     }
// }

template <class T>
void BTreeNode<T>::show()
{
    for (int i = 0; i < currKeys; i++)
        std::cout << keys[i] << " - ";

    std::cout << std::endl;
}

template class BTreeNode<int>;