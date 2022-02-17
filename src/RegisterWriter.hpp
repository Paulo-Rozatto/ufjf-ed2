#ifndef REGISTER_WRITER_H

#include <fstream>
#include <queue>
#include <math.h>
#include "./Register.hpp"

using namespace std;

int toInt(char *v, int n)
{
    int numb = 0;

    for (int i = 0; i < n; i++)
    {
        numb += (v[i] - '0') * pow(10, n - 1 - i);
    }

    return numb;
}

/*
    Todas as operacoes de escrita verificam de alguma forma se a informacao sendo lida esta completa no buffer.
    Se nao estiver, elas param a execucao e solicitam uma nova leitura do arquivo para o buffer.
    No buffer atualizado, o conteudo antes cortado estara no comeco e completo.
*/

bool writeID(char *buffer, int &pos, int length, fstream &file)
{
    bool needsNextBuffer = false;

    int size = length - pos;

    if (size <= 89)
    {
        needsNextBuffer = true;
    }
    else
    {
        size = 89;

        file.write((buffer + pos), size * sizeof(char));
        pos += size + 1;
    }

    return needsNextBuffer;
}

void writeReview(queue<string> &q, fstream &file)
{
    string r;
    int size;
    file.seekp(0, file.end);
    while (!q.empty())
    {
        r = q.front();
        size = r.size();
        file.write(reinterpret_cast<const char *>(&size), sizeof(int));
        file.write(r.c_str(), r.size() * sizeof(char));

        q.pop();
    }
}

/*
    Os textos de review podem comecar com algum caractere qualquer ou com aspas duplas.
    Se comeca com um char qualquer, a leitura e simples, ela acaba quando se encontra uma virgula.
    Se comeca com aspas, ela termina com aspas, mas nem todas aspas significam o final. 
    Duas aspas seguidas significam que ha aspas no texto, nao eh as aspas do final.
*/
bool reviewToQueue(char *buffer, int &pos, int length, queue<string> &q, fstream &file, int &reviewPos)
{
    bool needsNextBuffer = false;
    int i = 0, j, size;
    string text;

    if (buffer[pos] == '\"')
    {
        text += buffer[pos];
        i = pos + 1;

        while (true)
        {
            if (i == length)
            {
                needsNextBuffer = true;
                break;
            }

            text += buffer[i];

            if (buffer[i] == '\"')
            {
                if (buffer[i + 1] == '\"')
                {
                    i++;
                }
                else
                {
                    break;
                }
            }
            i++;
        }
        i++;
    }
    else
    {
        for (i = pos; buffer[i] != ','; i++)
        {
            if (i == length)
            {
                needsNextBuffer = true;
                break;
            }
            text += buffer[i];
        }
    }

    if (!needsNextBuffer)
    {
        // escreve onde sera enderco de onde esta o review
        file.write(reinterpret_cast<const char *>(&reviewPos), sizeof(reviewPos));

        q.push(text);

        // calcula a posicao para o proximo review
        reviewPos += text.size() * sizeof(char) + sizeof(int);

        pos = i + 1;
    }

    return needsNextBuffer;
}

bool writeUpVote(char *buffer, int &pos, int length, fstream &file)
{
    bool needsNextBuffer = false;
    int i, upvote;

    for (i = pos; buffer[i] != ','; i++)
    {
        if (i >= length)
        {
            needsNextBuffer = true;
            break;
        }
    }
    if (!needsNextBuffer)
    {
        upvote = toInt((buffer + pos), (i - pos));
        file.write(reinterpret_cast<const char *>(&upvote), sizeof(upvote));
        pos = i + 1;
    }

    return needsNextBuffer;
}

/*
    A versao e mantida em 3 inteiros terem o mesmo tamanho no arquivo binario.
    Caso nao seja informada a versao, o e atribuido 0.0.0
*/
bool writeVersion(char *buffer, int &pos, int length, fstream &file)
{
    bool needsNextBuffer = false;
    int i, start = pos;
    int v[3] = {0, 0, 0}, idx = 0;

    for (i = pos; buffer[i] != ','; i++)
    {
        if (i == length)
        {
            needsNextBuffer = true;
            break;
        }

        if (buffer[i] == '.')
        {
            v[idx] = toInt((buffer + start), (i - start));
            idx += 1;
            start = i + 1;
        }
    }
    if (!needsNextBuffer)
    {
        v[idx] = toInt((buffer + start), (i - start));

        for (int j = 0; j < 3; j++)
            file.write(reinterpret_cast<const char *>(v + j), sizeof(int));

        pos = i + 1;
    }

    return needsNextBuffer;
}

/*
    Como nao e feita nenuma operacao com a data, ela e mantida em array de char
*/
bool writeDate(char *buffer, int &pos, int length, fstream &file)
{
    bool needsNextBuffer = false;

    int size = length - pos;

    if (size <= 19)
    {
        needsNextBuffer = true;
    }
    else
    {
        size = 19;

        file.write((buffer + pos), size * sizeof(char));
        pos += 20;
    }

    return needsNextBuffer;
}

#endif