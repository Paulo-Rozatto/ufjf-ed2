#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <time.h>
#include <fstream>
#include <string.h>

#include "FileProcessing.hpp"
#include "Sort.hpp"
#include "BTree.hpp"
#include "BKey.hpp"
// #include "BTreeNode.hpp"
#include "ArvoreVP.h"

using namespace std;

// template class BTree<int, 5>;
// template class BTreeNode<int, 5>;

void importacao(Register **registers, int n)
{
    fstream bin;
    int *indices;
    bin.open("tiktok_app_reviews.bin", ios::in | ios::binary);

    srand(time(NULL));

    cout << "Escolhendo " << n << " registros aleatoriamente..." << endl;
    int j;
    for (int i = 0; i < n; i++)
    {
        j = rand() % ROWS;
        // j = i;
        registers[i]->initIdOnly(bin, j);
    }
}

Register **createArray(int n)
{
    Register **v;

    v = new Register *[n];
    for (int i = 0; i < n; i++)
        v[i] = new Register();

    return v;
}

void loadBKeys()
{
}

void arvB()
{
    // BTree<BKey> t(3);
    BTree<int> t(6);
    int size = 1000000;

    Register **r = createArray(size);
    // importacao(r, size);
    char nu[4] = {'n', 'a', 'd', '\0'};

    BKey *search;

    for (int i = 1; i <= size; i++)
    {
        // sprintf(nu, "%d", i);
        BKey key(i, nu);
        t.insert(size - i);

    }

    cout << endl
         << "Root: ";
    t.root->show();
    cout << endl;
    BTreeNode<int> *b;

    // for (int i = 0; i <= t.root->currKeys; i++)
    // {
    //     cout << "Child " << (i + 1) << ": ";
    //     t.root->children[i]->show();

    //     // for (int j = 0; j <= (t.root->children[i]->currKeys); j++)
    //     // {
    //     //     b = t.root->children[i]->children[j];
    //     //     t.root->children[i]->children[j]->show();
    //     // }
    // }
    // t.root->children[1]->children[0]->show();
    // t.root->children[1]->children[1]->show();

    // int key = 17;

    // cout << "Key: " << search->getId() << endl;
    BTreeNode<int> *s = t.root->search(388071);

    cout << "Encontrado: " << (s == nullptr ? "N" : "Y") << endl;
}

void arvoreVP()
{
    // ifstream input("input.dat");
    ofstream output("saida_hash.txt");

    // if(!input.good())
    // {
    //     cout << "Arquivo input.dat nao encontrado." << endl;
    //     input.close();
    //     return;
    // }

    if (!output.good())
    {
        cout << "Arquivo saida_hash.txt nao pode ser criado" << endl;
        output.close();
        return;
    }
    int n = 1000000;
    Register **r = createArray(n);
    importacao(r, n);

    ArvoreVP arv;
    for (int i = 0; i < n; i++)
    {
        string idReview = r[i]->getID();
        arv.insere(idReview, r[i]->getIndex());
    }
    cout << arv.getComparacaoInsercao() << " comparacaoInsercao " << endl;
    //arv.imprimePorNivel();
}

void teste()
{
    int n = 100;
    Register **r = createArray(n);
    ofstream output("teste.txt");
    ofstream outputHash("teste_Hash.txt");

    importacao(r, n);

    // testeOrdenacao(r, n, output);
    // testeHash(r, n, outputHash);

    // deleteArray(r, n);
}

int main(int argc, char const *argv[])
{
    char option;

    ifstream bin("tiktok_app_reviews.bin");
    if (!bin.good())
    {
        cout << "Arquivo binario nao encontrado." << endl;

        if (argc < 2)
        {
            cout << "Localizacao de tiktok_app_reviews.csv nao informado." << endl;
            return 1;
        }
        cout << "Tentando abrir " << argv[1] << endl;

        fstream csv;
        csv.open(argv[1], ios::in);

        if (csv.is_open())
        {
            std::chrono::time_point<std::chrono::system_clock> start, end;

            cout << "Processando " << argv[1] << " para tiktok_app_reviews.bin..." << endl;
            start = std::chrono::system_clock::now();
            csvToBin(csv);
            end = std::chrono::system_clock::now();

            std::chrono::duration<double> elapsed_seconds = end - start;
            cout << "Arquivo processado em " << elapsed_seconds.count() << "s" << endl;
        }
        else
        {
            cout << "Nao foi possivel abrir " << argv[1] << endl;
            return 1;
        }
    }

    do
    {
        cout << "Menu: " << endl
             << "1 - Arvore Vermelho e Preto: " << endl
             << "2 - Arvore B" << endl
             << "0 - Sair" << endl
             << "Digite a opcao: ";

        cin >> option;

        switch (option)
        {
        case '0':
            break;
        case '1':
            arvoreVP();
            break;
        case '2':
            // B();
            arvB();
            break;
        case '3':
            teste();
            break;
        default:
            cout << "Opcao invalida" << endl;
            break;
        }

    } while (option != '0');

    return 0;
}