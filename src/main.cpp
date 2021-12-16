#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <queue>
#include <chrono>
#include <time.h>
#include <cstdlib>

#include "FileProcessing.hpp"
#include "Avaliacao.hpp"


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
            cout << "A fazer..." << endl;
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