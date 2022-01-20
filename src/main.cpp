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
    for (int i = 0; i < n; i++)
        delete v[i];

    delete[] v;
}

void arvB(int size, int m)
{
    BTree<BKey> t(m);
    double insertTime = 0, searchTime = 0;
    int insertComp, insertMeanComp = 0;
    int searchComp, searchMeanComp = 0;
    ofstream outFile("saida.txt");

    Register **r = createArray(size);
    Register **b = createArray(100);

    outFile << "Arvore B:" << endl
            << "M: " << m << endl
            << "Insercoes: " << size << endl
            << "Buscas: 100" << endl;

    for (int trials = 0; trials < 3; trials++)
    {
        cout << endl
             << "Rodada " << (trials + 1) << "/3" << endl;
        outFile << endl
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

        outFile << "Insercoes" << endl
                << "  - Tempo: " << time << endl
                << "  - Comparacoes: " << insertComp << endl;

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

        outFile << "Busca" << endl
                << "  - Tempo: " << time << endl
                << "  - Comparacoes: " << insertComp << endl
                << endl;

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

    outFile << "Resutaldos finais: " << endl
            << "Insercao:" << endl
            << "  - Media de tempo: " << insertTime << endl
            << "  - Media de comparacoes: " << insertMeanComp << endl
            << "Busca" << endl
            << "  - Media de tempo: " << searchTime << endl
            << "  - Media de comparacoes: " << searchMeanComp << endl;

    deleteArray(r, size);
    deleteArray(b, 100);
}

void testeArvB(int size, int m)
{
    int aux = 0;
    BTree<BKey> t(m);

    Register **r = createArray(size);

    importacao(r, size);

    cout << "ID do primeiro registro sorteado: " << endl
         << "  - " << r[0]->getID() << endl
         << endl;

    cout << "Inserido registros na arvore" << endl;
    for (int i = 0; i < size; i++)
    {
        BKey key(r[i]->getIndex(), r[i]->getID());
        t.insert(key, &aux);
    }
    cout << "Registros inseridos" << endl;

    char id[90];
    cout << "Digite o id para busca: " << endl;
    cin >> id;

    BTreeNode<BKey> *search;
    BKey key(0, id);

    aux = 0;
    search = t.search(key, &aux);

    cout << endl;
    if (search == nullptr)
        cout << "Registro nao econtrado na arvore." << endl;
    else
        cout << "Registro foi encontrado!" << endl;
    cout << endl;

    deleteArray(r, size);
}

void arvoreVP()
{ 
    ifstream input("input.dat");
    ofstream output("saida_arvoreVP.txt");

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

    int escolha;

    do {
        cout << "Menu ARVORE VERMELHO-PRETO" << endl;
        cout << "Digite 1 para realizar a execução normal." << endl;
        cout << "Digite 2 para realizar o módulo de teste." << endl;
        cout <<  "Digite 0 para voltar." << endl;
        cin >> escolha;
    } while(escolha != 1 && escolha != 2 && escolha != 0);

    switch(escolha)
    {
        case 1:
        {
            float mediaInsercao = 0;
            float mediaBusca = 0;
            int numDeBuscasEncontrados = 0;
            for(int i = 0; i < 3; i++)
            {
                cout << "M" << (i+1) << endl;
                output << "- M" << (i + 1) << " -" << endl;
                int n = 1000000;
                Register **r = createArray(n);
                ArvoreVP arv;

                std::chrono::time_point<std::chrono::system_clock> start, end, start2, end2;

                
                start = std::chrono::system_clock::now();
                //seleção e inserção dos reviews na arvore
                fstream bin;
                bin.open("tiktok_app_reviews.bin", ios::in | ios::binary);

                srand(time(NULL));

                cout << "Escolhendo " << n << " registros aleatoriamente..." << endl;
                int j;
                string idReview;
                for (int i = 0; i < n; i++)
                {
                    j = rand() % ROWS;
                    r[i]->init(bin, j);
                    idReview = r[i]->getID();
                    arv.insere(idReview, j);
                }

                end = std::chrono::system_clock::now();

                std::chrono::duration<double> elapsed_seconds = end - start;
                cout << "Árvore gerada em " << elapsed_seconds.count() << "s" << endl;
                cout << arv.getnumInsercao() << " Comparacoes de Insercao " << endl;
                output << "Árvore gerada em " << elapsed_seconds.count() << "s" << endl;
                mediaInsercao += elapsed_seconds.count();
                output << arv.getnumInsercao() << " Comparações de Inserção" << endl;
            
                //busca
                bin.open("tiktok_app_reviews.bin", ios::in | ios::binary);
                string aux;
                srand(time(NULL));
                start = std::chrono::system_clock::now();
                int k;
                Register **review = createArray(100);
                for(int i = 0; i < 100; i++)
                {
                    k = rand() % ROWS;
                    review[i]->init(bin, k);
                    aux = review[i]->getID();
                    if(arv.busca(aux))
                    {
                        numDeBuscasEncontrados ++;
                    }
                }
                cout << "Numero de IDs encontrados: " << numDeBuscasEncontrados << endl;
                output << "Número de IDs encontrados: " << numDeBuscasEncontrados << endl;
                end = std::chrono::system_clock::now();

                elapsed_seconds = end - start;
                cout << "Busca de 100 reviews feita em " << elapsed_seconds.count() << "s" << endl;
                output << "Busca de 100 reviews feita em " << elapsed_seconds.count() << "s" << endl;
                mediaBusca += elapsed_seconds.count();
                output << arv.getnumBusca() << " Comparações de busca" << endl;
                deleteArray(r, n);
                deleteArray(review,100);
            }

            mediaBusca = mediaBusca / 3;
            mediaInsercao = mediaInsercao / 3;
            output << "Tempo médio de busca de 100 reviews: " << mediaBusca << "s" << endl;
            output << "Tempo médio de inserção de 1000000 reviews: " << mediaInsercao << "s" << endl;
            break;
        }
        case 2:
        {
                int n = 1000000;
                int j, verificaEscolha;
                Register **r = createArray(n);
                ArvoreVP arv;

                //seleção e inserção dos reviews na arvore
                fstream bin;
                bin.open("tiktok_app_reviews.bin", ios::in | ios::binary);

                srand(time(NULL));

                cout << "Deseja verificar se os ultimos 8 primeiros adicionados estao sendo encontrados pela funcao?" << endl;
                cout << "1 - SIM" << endl << "Qualquer outro numero - NAO" << endl;
                cin >> verificaEscolha;

                cout << "Escolhendo " << n << " registros aleatoriamente..." << endl;

                string idReview;

                for (int i = 0; i < n; i++)
                {
                    j = rand() % ROWS;
                    r[i]->init(bin, j);
                    idReview = r[i]->getID();
                    arv.insere(idReview, j);
                    if(i > 1 && i < 10 && verificaEscolha == 1)
                    {
                        idReview = r[i-1]->getID();
                        cout << "Inserindo review: " << idReview << endl;
                        if(arv.busca(idReview))
                    {
                        cout << "Review encontrado: " << idReview <<endl;
                    }
                        else
                        cout << "Nao achou" << endl;
                    }

                }
            
                //busca
                string aux;
                cout << "Informe o ID que deseja buscar" << endl;
                cin >> aux;
                srand(time(NULL));

                    if(arv.busca(aux))
                    {
                        cout << "Review encontrado!!" << endl;
                        cout << "Comparacoes de busca realizadas" << arv.getnumBusca() << endl;
                    }
                    else
                    {
                        cout << "Review nao encontrado!!" << endl;
                        cout << "Comparacoes de busca realizadas: " << arv.getnumBusca() << endl;
                    }
                deleteArray(r, n);
            break;
        }
    }

    


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
            int size = 1000000;

            cout << "Qual o tamanho maximo dos nos?" << endl;
            cin >> m;

            while (true)
            {
                cout << "1 - Modo relatorio " << endl
                     << "2 - Modo de teste" << endl
                     << "Digite a opcao: ";

                cin >> option;

                if (option == '1')
                {
                    arvB(size, m);
                    break;
                }
                else if (option == '2')
                {
                    testeArvB(size, m);
                    break;
                }
                else
                {
                    cout << "Opcao invalida" << endl;
                }
            }

            break;
        }
        default:
            cout << "Opcao invalida" << endl;
            break;
        }

    } while (option != '0');

    return 0;
}