#include "RegisterWriter.hpp"
#include <math.h>

int RegisterWriter::toInt(char *v, int n)
{
    int numb = 0;

    for (int i = 0; i < n; i++)
    {
        numb += (v[i] - '0') * pow(10, n - 1 - i);
    }

    return numb;
}

bool RegisterWriter::writeID(char *buffer, int &pos, int length, fstream &file)
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

void RegisterWriter::writeReview(queue<string> &q, fstream &file)
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

bool RegisterWriter::reviewToQueue(char *buffer, int &pos, int length, queue<string> &q, fstream &file, int &filePos)
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
        file.write(reinterpret_cast<const char *>(&filePos), sizeof(filePos));

        q.push(text);

        // calcula a posicao para o proximo review
        filePos += text.size() * sizeof(char) + sizeof(int);

        pos = i + 1;
    }

    return needsNextBuffer;
}

bool RegisterWriter::writeUpVote(char *buffer, int &pos, int length, fstream &file)
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

bool RegisterWriter::writeVersion(char *buffer, int &pos, int length, fstream &file)
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

bool RegisterWriter::writeDate(char *buffer, int &pos, int length, fstream &file)
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
