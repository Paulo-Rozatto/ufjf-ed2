#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <time.h>
#include <fstream>
#include <string.h>
#include <algorithm>
#include <unordered_map>

#include "FileProcessing.hpp"
#include "MinHeap.hpp"

using namespace std;

void importacao(Register **registers, int n)
{
    fstream bin;
    vector<int> indices(n);

    bin.open("tiktok_app_reviews.bin", ios::in | ios::binary);

    srand(time(NULL));

    cout << "Escolhendo " << n << " registros aleatoriamente..." << endl;

    for (int i = 0; i < n; i++)
        indices[i] = rand() % ROWS;

    sort(indices.begin(), indices.end());

    for (int i = 0; i < n; i++)
        registers[i]->init(bin, indices[i]);

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
    for (int i = 0; i < n; i++)
        delete v[i];

    delete[] v;
}

void fillFrequency(unordered_map<char, int> *frequencyMap, char *str, int n)
{
    unordered_map<char, int>::iterator it;

    for (int i = 0; i < n; i++)
    {
        it = frequencyMap->find(str[i]);

        if (it != frequencyMap->end())
            it->second++;

        else
            frequencyMap->insert(std::make_pair(str[i], 1));
    }
}

void fillTable(HuffNode *node, string code, unordered_map<char, string> *encodingTable)
{
    if (node == nullptr)
        return;

    if (node->getLeft() == nullptr && node->getRight() == nullptr)
    {
        (*encodingTable)[node->getKey()] = code;
    }
    else
    {
        fillTable(node->getRight(), code + '0', encodingTable);
        fillTable(node->getLeft(), code + '1', encodingTable);
    }
}

void encondigStructures(HuffNode *encodingTree, unordered_map<char, string> *encodingTable, unordered_map<char, int> *frequencyMap)
{
    MinHeap heap;

    // Insere caracteres e suas respectivas frequencias na heap
    for (pair<char, int> p : (*frequencyMap))
    {
        HuffNode *node = new HuffNode(p.first);
        node->setCount(p.second);

        heap.insert(node);
    }

    HuffNode *parent, *left, *right;

    // Monta a arvore de codificacao
    while (heap.getSize() > 1)
    {
        parent = new HuffNode();
        right = heap.popMin();
        left = heap.popMin();

        parent->setCount(right->getCount() + left->getCount());
        parent->setRight(right);
        parent->setLeft(left);

        heap.insert(parent);
    }

    encodingTree = heap.getRoot();

    // Usa a arvore para criar uma hash table com os codigos binarios para codificacao
    fillTable(encodingTree, "", encodingTable);

    // Descomentar para mostrar tabela de codificacao
    // for (pair<char, string> p : (*encodingTable))
    // {
    //     cout << p.first << ": " << p.second << endl;
    // }
}

void compressao()
{
    // const int SIZES[] = {10000, 100000, 1000000};
    const int SIZES[] = {10, 20, 30};
    int n;
    Register **registers;

    // Teste com valores do slide
    // A   C   E   D   T   O   B    F  G
    // 220 78  112 50  12  66  180  95 34
    // char arr[] = {'A', 'C', 'E', 'D', 'T', 'O', 'B', 'F', 'G'};
    // int freq[] = {220, 78, 112, 50, 12, 66, 180, 95, 34};

    for (int i = 0; i < 3; i++)
    {
        n = SIZES[i];

        registers = createArray(n);
        importacao(registers, n);

        unordered_map<char, int> frequencyMap;

        for (int j = 0; j < n; j++)
        {
            fillFrequency(&frequencyMap, registers[i]->getReview(), registers[i]->getReviewSize());
        }

        // Teste com valores do slide
        // for (int j = 0; j < 9; j++) {
        //     frequencyMap.insert(make_pair(arr[j], freq[j]));
        // }
        // for(auto p: frequencyMap) {
        //     cout << p.first << " - " << p.second << endl;
        // }
        // cout << endl;

        HuffNode encodingTree;
        unordered_map<char, string> encodingTable;

        encondigStructures(&encodingTree, &encodingTable, &frequencyMap);

        deleteArray(registers, n);
    }
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
             << "1 - Comprimir N reviews aleatorios " << endl
             << "2 - Descomprimir reviewsComp.bin" << endl

             << "0 - Sair" << endl
             << "Digite a opcao: ";

        cin >> option;

        switch (option)
        {
        case '0':
            break;
        case '1':
        {
            compressao();
            break;
        }
        case '2':
        {
            // descompressao
            break;
        }
        default:
            cout << "Opcao invalida" << endl;
            break;
        }

    } while (option != '0');

    return 0;
}