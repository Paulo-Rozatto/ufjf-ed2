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

void test()
{
    MinHeap heap(128);

    // A   C   E   D   T   O   B    F  G
    // 220 78  112 50  12  66  180  95 34

    char arr[] = {'A', 'C', 'E', 'D', 'T', 'O', 'B', 'F', 'G'};
    int freq[] = {220, 78, 112, 50, 12, 66, 180, 95, 34};

    for (int i = 0; i < 9; i++)
    {
        HuffNode *node = new HuffNode(arr[i]);
        node->setCount(freq[i]);

        heap.insert(node);
    }

    HuffNode *parent, *left, *right;

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

    HuffNode *encodingTree = heap.getRoot();
    unordered_map<char, string> encodingTable;

    fillTable(encodingTree, "", &encodingTable);

    for (auto pair : encodingTable)
    {
        cout << pair.first << ": " << pair.second << endl;
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

    test();

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
            // compressao
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