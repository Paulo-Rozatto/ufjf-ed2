#ifndef FILE_PROCESSING_HPP

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <queue>
#include "RegisterWriter.hpp"

#define BUFFER_SIZE 102400
#define ROWS 3646476;

using namespace std;

/*
    Código da parte 1 do trabalho
*/

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

void readFileToBuffer(char *buffer, int &length, fstream &file, int &pos, int fileSize)
{
    length = fileSize - pos;
    file.seekg(pos, file.beg);

    if (length > BUFFER_SIZE)
        length = BUFFER_SIZE;

    file.seekg(pos);
    file.read(buffer, length);
}

void csvToBin(fstream &csv)
{
    fstream out;

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

    // Calculando tamanho da linha
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

    enum
    {
        ID,
        REVIEW,
        UPVOTE,
        VERSION,
        DATE
    };

    int nextReviewPos = ROW_SIZE * ROWS;
    int next = ID, pos = 0, filePos = 54;
    bool needsNextBuffer;
    queue<string> reviewQueue;

    while (pos < bufferLength)
    {
        switch (next)
        {
        case ID:
        {
            // as funcoes de escrita foram colocadas em um arquivo separado para evitar poluir muito o arquivo main.cpp
            needsNextBuffer = writeID(buffer, pos, bufferLength, out);
            if (needsNextBuffer)
                break;

            next = REVIEW;
        }

        case REVIEW:
        {
            needsNextBuffer = reviewToQueue(buffer, pos, bufferLength, reviewQueue, out, nextReviewPos);
            if (needsNextBuffer)
                break;

            next = UPVOTE;
        }

        case UPVOTE:
        {
            needsNextBuffer = writeUpVote(buffer, pos, bufferLength, out);
            if (needsNextBuffer)
                break;

            next = VERSION;
        }

        case VERSION:
        {
            needsNextBuffer = writeVersion(buffer, pos, bufferLength, out);
            if (needsNextBuffer)
                break;

            next = DATE;
        }

        case DATE:
        {
            needsNextBuffer = writeDate(buffer, pos, bufferLength, out);
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
            readFileToBuffer(buffer, bufferLength, csv, filePos, FILE_SIZE);
            pos = 0;
        }
    }

    writeReview(reviewQueue, out);

    csv.close();
    out.close();
    delete[] buffer;
}

#endif