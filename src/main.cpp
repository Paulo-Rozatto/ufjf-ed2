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
#include "ArvoreVP.h"

using namespace std;

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
        registers[i]->initIdOnly(bin, j);
    }

    cout << "Registros escolhidos!" << endl
         << endl;
}

Register **createArray(int n)
{
    Register **v;

    v = new Register *[n];
    for (int i = 0; i < n; i++)
        v[i] = new Register();

    return v;
}

void deleteArray(Register **v, int n)
{
    // for (int i = 0; i < n; i++)
    //     delete v[i];

    delete[] v;
}

void arvB(int size, int m)
{
    BTree<BKey> t(m);
    double insertTime = 0, searchTime = 0;
    int insertComp, insertMeanComp = 0;
    int searchComp, searchMeanComp = 0;

    Register **r = createArray(size);
    Register **b = createArray(100);

    for (int trials = 0; trials < 3; trials++)
    {
        cout << endl
             << "Rodada " << (trials + 1) << "/3" << endl;

        importacao(r, size);

        insertComp = 0;
        searchComp = 0;

        std::chrono::time_point<std::chrono::system_clock> start, end;
        double time;

        cout << "Inserindo " << size << " chaves em uma arvore B..." << endl;
        start = std::chrono::system_clock::now();
        for (int i = 0; i < size; i++)
        {
            BKey key(r[i]->getIndex(), r[i]->getID());
            t.insert(key, &insertComp);
        }
        end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed_seconds = end - start;
        time = elapsed_seconds.count();
        cout << "Registros inseridos em " << time << "s " << endl
             << insertComp << " comparacoes entre chaves feitas" << endl
             << endl;
        insertTime += time;
        insertMeanComp += insertComp;

        // -- Busca -- //
        std::chrono::time_point<std::chrono::system_clock> start2, end2;

        cout << "Gerar registros para busca" << endl;
        importacao(b, 100);

        BTreeNode<BKey> *search;

        cout << "Buscando 100 registros..." << endl;
        int encontrados = 0;
        time = 0;

        BKey key;
        start2 = std::chrono::system_clock::now();
        for (int i = 0; i < 100; i++)
        {
            key.init(b[i]->getIndex(), b[i]->getID());
            // search = t.root->search(key, &searchComp);
            search = t.search(key, &searchComp);

            if (search != nullptr)
                encontrados++;
        }
        end2 = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed_seconds2 = end2 - start2;
        time = elapsed_seconds2.count();

        cout << "100 registros buscados e " << encontrados << " encontrados em " << time << "s" << endl
             << searchComp << " comparacoes entre chaves feitas" << endl
             << endl;
        searchTime += time;
        searchMeanComp += searchComp;

        cout << "Limpando memoria..." << endl;
    }

    insertTime /= 3;
    searchTime /= 3;
    insertMeanComp /= 3;
    searchMeanComp /= 3;

    cout << "Tempo medio de insercao: " << insertTime << "s" << endl;
    cout << "Tempo medio de busca: " << searchTime << "s" << endl;
    cout << "Comparacoes media de insercao: " << insertMeanComp << endl;
    cout << "Comparacoes media de busca: " << searchMeanComp << endl;

    // deleteArray(r, size);
    // deleteArray(b, 100);
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
        {
            arvoreVP();
            break;
        }
        case '2':
        {
            int m;
            int size = 200;

            cout << "Qual o tamanho maximo dos nos?" << endl;
            cin >> m;

            arvB(size, m);
            break;
        }
        default:
            cout << "Opcao invalida" << endl;
            break;
        }

    } while (option != '0');

    return 0;
}