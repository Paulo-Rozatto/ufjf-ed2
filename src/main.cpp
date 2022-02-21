#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <time.h>
#include <fstream>
#include <string.h>
#include <algorithm>
#include <unordered_map>

#include "FileProcessing.hpp"
#include "ImportHelper.hpp"
#include "MinHeap.hpp"

using namespace std;

/**
 * Dada uma string, conta a frequencia de cada caracter usando o map(hash table)
 */
void fillFrequency(unordered_map<char, int> *frequencyMap, string str)
{
    unordered_map<char, int>::iterator it;

    for (int i = 0; i < str.length(); i++)
    {
        it = frequencyMap->find(str[i]);

        if (it != frequencyMap->end())
            it->second++;

        else
            frequencyMap->insert(std::make_pair(str[i], 1));
    }
}

/**
 * A partir da arvore de compressao, associa cada caracere com seu codigo binario
 */
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

/**
 * Constroi heap, entao a arvore de codificacao e chama a construcao do tabela feita no metodo acima
 */
void encondigStructures(HuffNode **encodingTree, unordered_map<char, string> *encodingTable, unordered_map<char, int> *frequencyMap)
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

    *encodingTree = heap.getRoot();

    // Usa a arvore para criar uma hash table com os codigos binarios para codificacao
    fillTable(*encodingTree, "", encodingTable);
}

string copiaRegistrosParaString(Register **registers, int tam)
{
    string texto;
    for (int i = 0; i < tam; i++)
    {
        texto += registers[i]->getReview();
        texto += '\n';
    }
    return texto;
}

string decodificar(string texto, HuffNode *raiz)
{
    int i = 0;
    HuffNode *aux = raiz;
    string decodificado;

    while (texto[i] != '\0')
    {
        if (texto[i] == '0')
            aux = aux->getRight();
        else
            aux = aux->getLeft();

        if (aux->getLeft() == NULL && aux->getRight() == NULL)
        {
            decodificado += aux->getKey();
            aux = raiz;
        }
        i++;
    }
    return decodificado;
}

unsigned int eh_bit_um(unsigned char byte, int i)
{
    unsigned char mascara = (1 << i);
    return byte & mascara;
}

void descomprimeEscreveBin(HuffNode *raiz)
{
    FILE *arq = fopen("reviewsComp.bin", "rb");
    FILE *arquivo = fopen("reviewsOrig.bin", "wb");
    ofstream outFile("saida.txt");
    HuffNode *aux = raiz;
    unsigned char byte, auxChar;
    int i;
    string texto;
    if (arq)
    {
        // enquanto conseguir ler do arquivo
        while (!feof(arq))
        {
            fread(&byte, sizeof(unsigned char), 1, arq);
            for (i = 7; i >= 0; i--)
            {
                if ((eh_bit_um(byte, i)))
                {
                    aux = aux->getLeft();
                }
                else
                {
                    aux = aux->getRight();
                }
                if (aux->getLeft() == nullptr && aux->getRight() == nullptr)
                {
                    // cout << aux->getKey();
                    auxChar = aux->getKey();
                    fwrite(&auxChar, sizeof(unsigned char), 1, arquivo);
                    aux = raiz;
                }
            }
        }
        fclose(arq);
    }
    else
        printf("\nErro ao abrir arquivo em decomprime\n");
}

void comprimeEscreveBin(string texto)
{
    FILE *arq = fopen("reviewsComp.bin", "wb");

    if (arq)
    {
        int j = 7;
        unsigned char mascara, byte = 0;
        for (int i = 0; i < texto.length(); i++)
        {
            mascara = 1;
            if (texto[i] != '0')
            {
                mascara = mascara << j;
                byte = byte | mascara;
            }
            j--;

            if (j < 0)
            {
                fwrite(&byte, sizeof(unsigned char), 1, arq);
                byte = 0;
                j = 7;
            }
        }
        if (j != 7) // tem um byte em formação
        {
            fwrite(&byte, sizeof(unsigned char), 1, arq);
        }
        fclose(arq);
    }

    else
    {
        cout << "Nao foi possivel criar o arquivo binario" << endl;
    }
}

void compressaoNEscolhas()
{
    int n;
    Register **registers;
    cout << "Informe o numero N de reviews" << endl;
    cin >> n;
    registers = createArray(n);
    importacao(registers, n);

    unordered_map<char, int> frequencyMap;
    string texto = copiaRegistrosParaString(registers, n);
    fillFrequency(&frequencyMap, texto);

    HuffNode *encodingTree;
    unordered_map<char, string> encodingTable;

    encondigStructures(&encodingTree, &encodingTable, &frequencyMap);

    // Copia as informações dos registros para uma string e desaloca
    deleteArray(registers, n);

    string aux, novoTexto;
    for (int i = 0; i < texto.length(); i++)
    {
        aux += encodingTable[texto[i]];
    }

    comprimeEscreveBin(aux);

    ifstream file("reviewsComp.bin", ios::ate | ios::binary);
    int originalSize = texto.length() * sizeof(char);
    int compressedSize = file.tellg();

    cout << "Tamanho original: " << originalSize << " bytes" << endl;
    cout << "Tamanho comprimido: " << compressedSize << " bytes" << endl;
    cout << "Taxa compressao: " << (originalSize - compressedSize) / (float)originalSize * 100 << "%" << endl;

    delete encodingTree;
}

void compressao()
{
    const int SIZES[] = {10000, 100000, 1000000};
    int n;
    Register **registers;

    for (int i = 0; i < 3; i++)
    {
        n = SIZES[i];

        registers = createArray(n);
        importacao(registers, n);

        unordered_map<char, int> frequencyMap;
        string texto = copiaRegistrosParaString(registers, n);

        fillFrequency(&frequencyMap, texto);

        HuffNode *encodingTree;
        unordered_map<char, string> encodingTable;

        encondigStructures(&encodingTree, &encodingTable, &frequencyMap);

        // Copia as informações dos registros para uma string e desaloca
        deleteArray(registers, n);

        string aux, novoTexto;
        for (int i = 0; i < texto.length(); i++)
        {
            aux += encodingTable[texto[i]];
        }

        comprimeEscreveBin(aux);
        novoTexto = decodificar(aux, encodingTree);
        descomprimeEscreveBin(encodingTree);

        ifstream file("reviewsComp.bin", ios::ate | ios::binary);
        int originalSize = texto.length() * sizeof(char);
        int compressedSize = file.tellg();

        cout << "Tamanho original: " << originalSize << " bytes" << endl;
        cout << "Tamanho comprimido: " << compressedSize << " bytes" << endl;
        cout << "Taxa compressao: " << (originalSize - compressedSize) / (float)originalSize * 100 << "%" << endl;

        delete encodingTree;
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
             << "2 - Executar sequencia de N compreesoes(10000, 100000, 1000000" << endl
             << "0 - Sair" << endl
             << "Digite a opcao: ";

        cin >> option;

        switch (option)
        {
        case '0':
            break;
        case '1':
        {
            compressaoNEscolhas();
            break;
        }
        case '2':
        {
            compressao();
        }
        default:
            cout << "Opcao invalida" << endl;
            break;
        }

    } while (option != '0');

    return 0;
}