#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <time.h>
#include <fstream>
#include <string.h>

#include "FileProcessing.hpp"
#include "Sort.hpp"
#include "BTree.hpp"
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

void arvB()
{
    // BTreeNode<int> b(4, true);
    BTree<int, 5> t;

    t.insert(4);
    t.insert(2);
    t.insert(1);
    t.insert(5);
    t.insert(3);
    t.insert(6);
    t.insert(7);
    t.insert(8);
    t.insert(9);
    t.insert(11);
    t.insert(12);
    t.insert(13);
    t.insert(14);
    t.insert(15);
    t.insert(16);
    t.insert(17);
    t.insert(18);

    cout << endl
         << "Root: ";
    t.root->show();
    cout << endl;

    for (int i = 0; i <= t.root->currKeys; i++)
    {
        cout << "Child " << (i + 1) << ": ";
        t.root->children[i]->show();
    }

    int key = 17;

    cout << "Key: " << key << endl;
    BTreeNode<int> *s = t.root->search(key);

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

    if(!output.good())
    {
        cout << "Arquivo saida_hash.txt nao pode ser criado" << endl;
        output.close();
        return;
    }
    int n = 1000000;
    Register **r = createArray(n);
    importacao(r, n);
    
    ArvoreVP arv;
    for(int i = 0; i < n; i++)
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