#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <time.h>
#include <fstream>

#include "FileProcessing.hpp"
#include "Avaliacao.hpp"
#include "Sort.hpp"

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
        // registers[i] = new Register(bin, j);
        registers[i]->init(bin, j);
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

void copyUpvotes(Register **src, Register **dst, int n)
{
    for (int i = 0; i < n; i++)
    {
        dst[i]->setUpvote(src[i]->getUpvote());
    }
}

void deleteArray(Register **v, int n)
{
    for (int i = 0; i < n; i++)
        delete v[i];

    delete[] v;
}

void ordenacao()
{
    ifstream input("input.dat");
    ofstream output("saida.txt");

    if (!input.good())
    {
        cout << "Arquivo input.dat nao encontrado." << endl;
        input.close();
        return;
    }

    if (!output.good())
    {
        cout << "Arquivo saida.txt nao pode ser criado" << endl;
        output.close();
        return;
    }

    Register **v1, **v2, **v3;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;
    int n, m = 3, contMoves = 0, contComps = 0;
    double t1, t2, t3;
    int cm1, cm2, cm3, cc1, cc2, cc3;

    while (input >> n)
    {
        v1 = createArray(n);
        v2 = createArray(n);
        v3 = createArray(n);

        t1 = t2 = t3 = cm1 = cm2 = cm3 = cc1 = cc2 = cc3 = 0;

        cout << "--- Teste para " << n << " valores. ---" << endl;
        output << "----- " << n << " valores -----" << endl;

        for (int i = 0; i < m; i++)
        {
            cout << "M" << (i + 1) << endl;
            output << "- M" << (i + 1) << " -" << endl;

            importacao(v1, n);
            copyUpvotes(v1, v2, n);
            copyUpvotes(v1, v3, n);

            cout << endl;

            cout << "Quick sort... ";
            output << "Quick sort" << endl;
            contMoves = 0, contComps = 0;

            start = std::chrono::system_clock::now();
            quickSort(v1, 0, n, &contMoves, &contComps);
            end = std::chrono::system_clock::now();

            elapsed_seconds = end - start;
            t1 += elapsed_seconds.count();
            cm1 += contMoves;
            cc1 += contComps;

            cout << "Concluindo em " << t1 << " segundos" << endl
                 << endl;

            output << "Movimentacoes: " << contMoves << endl;
            output << "Comparacoes: " << contComps << endl;
            output << "Concluindo em " << t1 << " segundos" << endl
                   << endl;
            cout << "Heap sort... ";
            output << "Heap sort" << endl;
            contMoves = 0, contComps = 0;

            start = std::chrono::system_clock::now();
            heapSort(v1, 0, n, &contMoves, &contComps);
            end = std::chrono::system_clock::now();

            elapsed_seconds = end - start;
            t2 += elapsed_seconds.count();
            cm2 += contMoves;
            cc2 += contComps;

            cout << "Concluindo em " << t2 << " segundos" << endl
                 << endl;

            output << "Movimentacoes: " << contMoves << endl;
            output << "Comparacoes: " << contComps << endl;
            output << "Concluindo em " << t2 << " segundos" << endl
                   << endl;

            cout << "Intro sort... ";
            output << "Intro sort" << endl;
            contMoves = 0, contComps = 0;

            start = std::chrono::system_clock::now();
            introSort(v1, 0, n, &contMoves, &contComps);
            end = std::chrono::system_clock::now();

            elapsed_seconds = end - start;
            t3 += elapsed_seconds.count();
            cm3 += contMoves;
            cc3 += contComps;

            cout << "Concluindo em " << t3 << " segundos" << endl
                 << endl;
            output << "Movimentacoes: " << contMoves << endl;
            output << "Comparacoes: " << contComps << endl;
            output << "Concluindo em " << t2 << " segundos" << endl
                   << endl;
        }

        output << "Médias" << endl;

        // cout << "QuickSort " << endl
        //      << "Movimentacoes: " << cm1 / (float)m << endl
        //      << "Comparacoes: " << cc1 / (float)m << endl
        //      << "Tempo: " << t1 / (float)m << endl
        //      << endl;
        // cout << "HeapSort " << endl
        //      << "Movimentacoes: " << cm2 / (float)m << endl
        //      << "Comparacoes: " << cc2 / (float)m << endl
        //      << "Tempo: " << t2 / (float)m << endl
        //      << endl;
        // cout << "IntroSort: " << endl
        //      << "Movimentacoes: " << cm3 / (float)m << endl
        //      << "Comparacoes: " << cc3 / (float)m << endl
        //      << "Tempo: " << t3 / (float)m << endl
        //      << endl;

        output << "QuickSort: " << endl
               << "Movimentacoes: " << cm1 / (float)m << endl
               << "Comparacoes: " << cc1 / (float)m << endl
               << "Tempo: " << t1 / (float)m << endl
               << endl;
        output << "HeapSort: " << endl
               << "Movimentacoes: " << cm2 / (float)m << endl
               << "Comparacoes: " << cc2 / (float)m << endl
               << "Tempo: " << t2 / (float)m << endl
               << endl;
        output << "IntroSort: " << endl
               << "Movimentacoes: " << cm3 / (float)m << endl
               << "Comparacoes: " << cc3 / (float)m << endl
               << "Tempo: " << t3 / (float)m << endl
               << endl;

        deleteArray(v1, n);
        deleteArray(v2, n);
        deleteArray(v3, n);

        cout << endl;
    }
}

void temporario()
{
    int n = 10;
    Register **r = createArray(n);
    int contMov = 0, contComp = 0;
    // r = new Register *[n];

    importacao(r, n);

    for (int i = 0; i < n; i++)
        cout << r[i]->getUpvote() << " ";
    cout << endl;

    introSort(r, 0, n, &contMov, &contComp);

    for (int i = 0; i < n; i++)
        cout << r[i]->getUpvote() << " ";
    cout << endl;

    cout << "Mov: " << contMov << " - "
         << "Comp: " << contComp << endl;
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
             << "1 - Ordenacao: " << endl
             << "2 - Hash" << endl
             << "3 - Modulo de teste:" << endl
             << "0 - Sair" << endl
             << "Digite a opcao: ";

        cin >> option;

        switch (option)
        {
        case '0':
            break;
        case '1':
            ordenacao();
            break;
        case '2':
            cout << "A fazer..." << endl;
            break;
        case '3':
            cout << "A fazer..." << endl;
            break;
        default:
            cout << "Opcao invalida" << endl;
            break;
        }

    } while (option != '0');

    return 0;
}