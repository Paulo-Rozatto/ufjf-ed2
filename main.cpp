#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <queue>
#include <chrono>
#include <time.h>
#include "Register.hpp"
#include "RegisterWriter.hpp"

#define BUFFER_SIZE 102400
#define ROWS 3646476;

using namespace std;

/*
    Os dados da tabela tem todos o mesmo tamanho, tirando o review.
    Alem disso a quantidade de dados e fixa, o arquivo sempre eh o mesmo com 3.6 milhoes de linhas.
    Entao, lemos e salvamos primeiro os dados fixos e vamos adcionando os reviews em uma fila.
    No final, e so escrever os reviews da fila no arquivo.
    Conseguimos calcular com antecendencia em qual posicao do arquivo o review vai ficar.
    Entao o binario fica organizado mais ou menos assim.

    id1 posicao_do_review1 upvote1 version1 date1
    id2 posicao_do_review2 upvote2 version2 date2
    id3 posicao_do_review3 upvote3 version3 date3
    [...]
    tamanho_do_review1 review1
    tamanho_do_review2 review2
    tamanho_do_review3 review3
    [...]
*/

enum
{
    ID,
    REVIEW,
    UPVOTE,
    VERSION,
    DATE
};

void readFileToBuffer(char *buffer, int &length, fstream &file, int &pos, int fileSize)
{
    length = fileSize - pos;
    file.seekg(pos, file.beg);

    if (length > BUFFER_SIZE)
        length = BUFFER_SIZE;

    file.seekg(pos);
    file.read(buffer, length);
}

void process(fstream &csv)
{
    fstream out;

    // csv.open("archive/test.txt", ios::in);
    // csv.open("archive/tiktok_app_reviews.csv", ios::in);
    out.open("tiktok_app_reviews.bin", ios::out | ios::binary);

    if (!out.is_open())
    {
        cout << "Erro: Nao foi possivel criar arquivo tiktok_app_reviews.bin" << endl;
        csv.close();
        out.close();
        return;
    }

    csv.seekg(0, csv.end);
    const int FILE_SIZE = csv.tellg();

    // id (89 char) + review position (1 int) + upvote (1 int) + version (3 int) + date (19 char)
    const int ROW_SIZE = 89 * sizeof(char) + sizeof(int) + sizeof(int) + 3 * sizeof(int) + 19 * sizeof(char);

    int bufferLength;

    if (FILE_SIZE > BUFFER_SIZE)
        bufferLength = BUFFER_SIZE;
    else
        bufferLength = FILE_SIZE;

    char *buffer = new char[bufferLength];

    csv.seekg(54, csv.beg); // pula primeira linha do arquivo com o nome das colunas
    csv.read(buffer, bufferLength);

    int rowPos = ROW_SIZE * ROWS;
    int next = ID, pos = 0, filePos = 54;
    bool needsNextBuffer;
    queue<string> reviewQueue;

    while (pos < bufferLength)
    {
        switch (next)
        {
        case ID:
        {
            needsNextBuffer = RegisterWriter::writeID(buffer, pos, bufferLength, out);
            if (needsNextBuffer)
                break;

            next = REVIEW;
        }

        case REVIEW:
        {
            needsNextBuffer = RegisterWriter::reviewToQueue(buffer, pos, bufferLength, reviewQueue, out, rowPos);
            if (needsNextBuffer)
                break;

            next = UPVOTE;
        }

        case UPVOTE:
        {
            needsNextBuffer = RegisterWriter::writeUpVote(buffer, pos, bufferLength, out);
            if (needsNextBuffer)
                break;

            next = VERSION;
        }

        case VERSION:
        {
            needsNextBuffer = RegisterWriter::writeVersion(buffer, pos, bufferLength, out);
            if (needsNextBuffer)
                break;

            next = DATE;
        }

        case DATE:
        {
            needsNextBuffer = RegisterWriter::writeDate(buffer, pos, bufferLength, out);
            if (needsNextBuffer)
                break;

            next = ID;
        }
        }

        if (pos >= bufferLength && csv.tellg() < FILE_SIZE)
        {
            needsNextBuffer = true;
        }

        if (needsNextBuffer)
        {
            filePos += pos;
            // cout << filePos / 543022558.0 << endl; // porcentagem do arquivo processado
            readFileToBuffer(buffer, bufferLength, csv, filePos, FILE_SIZE);
            pos = 0;
        }
    }

    RegisterWriter::writeReview(reviewQueue, out);

    csv.close();
    out.close();
    delete[] buffer;
}

void acessaRegistro(int i)
{
    fstream bin;
    bin.open("tiktok_app_reviews.bin", ios::in | ios::binary);

    Register r(bin, i);

    r.print();
    bin.close();
}

void testeImportacao()
{
    fstream bin;
    Register *registers;
    char c;
    int n;
    bin.open("tiktok_app_reviews.bin", ios::in | ios::binary);

    do
    {
        cout << "Qual opcao: " << endl
             << "1 - Exibir no console" << endl
             << "2 - Gerar arquivo de texto" << endl
             << "opcao: ";
        cin >> c;

        if (c != '1' && c != '2')
            cout << "Opcao invalida!" << endl;

    } while (c != '1' && c != '2');

    srand(time(NULL));

    n = c == '1' ? 10 : 100;
    registers = new Register[n];

    cout << "Escolhendo " << n << " registros aleatoriamente..." << endl;
    int j;
    for (int i = 0; i < n; i++)
    {
        j = rand() % ROWS;
        registers[i].init(bin, j);
    }

    if (c == '1')
    {
        for (int i = 0; i < n; i++)
        {
            cout << (i + 1) << " - Registro " << j << ":" << endl;
            registers[i].print();
            cout << endl;
        }
    }
    else
    {
        fstream out;
        out.open("teste_importacao.txt", ios::out);

        cout << "Escrevendo " << n << " registros em teste_importacao.txt" << endl;
        for (int i = 0; i < n; i++)
        {
            out << (i + 1) << " - Registro " << j << ":" << endl;
            registers[i].print(out);
            out << endl;
        }
    }

    delete[] registers;
}

void importacao()
{
    fstream bin;
    Register *registers;
    char c;
    int n;
    bin.open("tiktok_app_reviews.bin", ios::in | ios::binary);

    cout << "Digite o valor de N: ";
    cin >> n;

    registers = new Register[n];

    do
    {
        cout << "Onde exibir? " << endl
             << "1 - Exibir no console" << endl
             << "2 - Gerar arquivo de texto" << endl
             << "opcao: ";
        cin >> c;

        if (c != '1' && c != '2')
            cout << "Opcao invalida!" << endl;

    } while (c != '1' && c != '2');

    srand(time(NULL));

    cout << "Escolhendo " << n << " registros aleatoriamente..." << endl;
    int j;
    for (int i = 0; i < n; i++)
    {
        j = rand() % ROWS;
        registers[i].init(bin, j);
    }

    if (c == '1')
    {
        for (int i = 0; i < n; i++)
        {
            cout << (i + 1) << " - Registro " << j << ":" << endl;
            registers[i].print();
            cout << endl;
        }
    }
    else
    {
        fstream out;
        string name = "importacao_" + to_string(n) + "_registros.txt";
        out.open(name, ios::out);

        cout << "Escrevendo " << n << " registros em " << name << "..." << endl;
        for (int i = 0; i < n; i++)
        {
            out << (i + 1) << " - Registro " << j << ":" << endl;
            registers[i].print(out);
            out << endl;
        }
    }

    delete[] registers;
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
            process(csv);
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
             << "1 - Acessar Registro: " << endl
             << "2 - Teste de Importacao:" << endl
             << "3 - Importar N registros" << endl
             << "0 - Sair" << endl
             << "Digite a opcao: ";

        cin >> option;

        switch (option)
        {
        case '0':
            break;
        case '1':
            int i;
            cout << "Informe o indice do registro: ";
            cin >> i;
            acessaRegistro(i);
            break;
        case '2':
            testeImportacao();
            break;
        case '3':
            importacao();
            break;
        default:
            cout << "Opcao invalida" << endl;
            break;
        }

    } while (option != '0');

    return 0;
}