#include <iostream>
#include <fstream>
#include <math.h>

#define BUFFER_SIZE 102400
// #define BUFFER_SIZE 142
#define ROWS 3646473

using namespace std;

enum
{
    ID,
    REVIEW,
    UPVOTE,
    VERSION,
    DATE
};

int toInt(char *v, int n)
{
    int numb = 0;

    for (int i = 0; i < n; i++)
    {
        numb += (v[i] - '0') * pow(10, n - 1 - i);
    }

    return numb;
}

bool writeID(char *buffer, int &pos, int length, fstream &file)
{
    bool needsNextBuffer = false;

    int size = length - pos;

    if (size < 89)
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

bool writeReview(char *buffer, int &pos, int length, fstream &file, int &row)
{
    bool needsNextBuffer = false;
    int i = 0, size;

    if (buffer[pos] == '\"')
    {
        i = pos + 1;

        while (true)
        {
            if (i == length)
            {
                needsNextBuffer = true;
                break;
            }

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
        for (i += pos; buffer[i] != ','; i++)
        {
            if (i == length)
            {
                needsNextBuffer = true;
                break;
            }
        }
    }

    if (!needsNextBuffer)
    {
        size = i - pos;
        file.write(reinterpret_cast<const char *>(&row), sizeof(row));

        int aux = file.tellp();
        file.seekp(row, file.beg);
        file.write(reinterpret_cast<const char *>(&size), sizeof(size));
        file.write(buffer + pos, size * sizeof(char));
        row += size + sizeof(size);
        file.seekp(aux, file.beg);
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
        if (i == length)
        {
            needsNextBuffer = true;
            break;
        }
    }

    upvote = toInt((buffer + pos), (i - pos));
    file.write(reinterpret_cast<const char *>(&upvote), sizeof(upvote));
    pos = i + 1;
    return needsNextBuffer;
}

bool writeVersion(char *buffer, int &pos, int length, fstream &file)
{
    bool needsNextBuffer = false;
    int i, numb, count = 0;

    for (i = pos; buffer[i] != ','; i++)
    {
        if (i == length)
        {
            needsNextBuffer = true;
            break;
        }

        if (buffer[i] == '.')
        {
            numb = toInt((buffer + pos), (i - pos));
            file.write(reinterpret_cast<const char *>(&numb), sizeof(numb));
            pos = i + 1;
            count++;
        }
    }
    if (count == 2)
    {
        numb = toInt((buffer + pos), (i - pos));
        file.write(reinterpret_cast<const char *>(&numb), sizeof(numb));
    }
    else
    {
        numb = 0;
        for (int j = count; j < 3; j++)
            file.write(reinterpret_cast<const char *>(&numb), sizeof(numb));
    }
    pos = i + 1;

    return needsNextBuffer;
}

bool writeDate(char *buffer, int &pos, int length, fstream &file)
{
    bool needsNextBuffer = false;

    int size = length - pos;

    if (size < 19)
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

void readToBuffer(char *buffer, int &length, fstream &file, int &pos)
{
    file.seekg(0, file.end);
    length = file.tellg();
    length = length - pos;
    file.seekg(pos, file.beg);

    if (length > BUFFER_SIZE)
        length = BUFFER_SIZE;

    file.seekg(pos, file.beg);
    file.read(buffer, length);
}

void process()
{
    fstream csv, out;

    csv.open("archive/tiktok_app_reviews.csv", ios::in | ios::binary);
    // csv.open("archive/test.txt", ios::in | ios::binary);
    out.open("archive/tiktok_app_reviews.bin", ios::out | ios::binary);

    if (!csv.is_open())
    {
        cout << "Erro: Arquivo archive/tiktok_app_reviews.csv nao encontrado" << endl;
        csv.close();
        return;
    }

    csv.seekg(0, csv.end);
    int length = csv.tellg();
    csv.seekg(54, csv.beg);

    if (length > BUFFER_SIZE)
        length = BUFFER_SIZE;

    char *buffer = new char[length];

    csv.read(buffer, length);

    char test = buffer[0];

    // id (89 char) + review position (1 int) + upvote (1 int) + version (3 int) + date (19 char)
    const int ROW_SIZE = 89 * sizeof(char) + sizeof(int) + sizeof(int) + 3 * sizeof(int) + 19 * sizeof(char);
    int rowPos = ROW_SIZE * ROWS;
    int next = ID, pos = 0, filePos = 0;
    bool needsNextBuffer;

    while (pos < length)
    {
        switch (next)
        {
        case ID:
        {
            needsNextBuffer = writeID(buffer, pos, length, out);
            if (needsNextBuffer)
                break;

            next = REVIEW;
        }

        case REVIEW:
        {
            needsNextBuffer = writeReview(buffer, pos, length, out, rowPos);
            if (needsNextBuffer)
                break;

            next = UPVOTE;
        }

        case UPVOTE:
        {
            needsNextBuffer = writeUpVote(buffer, pos, length, out);
            if (needsNextBuffer)
                break;

            next = VERSION;
        }

        case VERSION:
        {
            needsNextBuffer = writeVersion(buffer, pos, length, out);
            if (needsNextBuffer)
                break;

            next = DATE;
        }

        case DATE:
        {
            needsNextBuffer = writeDate(buffer, pos, length, out);
            if (needsNextBuffer)
                break;

            next = ID;
        }
        }

        if (needsNextBuffer)
        {
            filePos += pos;
            readToBuffer(buffer, length, csv, filePos);
            pos = 0;
        }
    }

    // writeID(buffer, pos, length, out);
    // writeReview(buffer, pos, length, out);
    // writeUpVote(buffer, pos, length, out);
    // writeVersion(buffer, pos, length, out);
    // writeDate(buffer, pos, length, out);

    csv.close();
    delete[] buffer;
}

int main(int argc, char const *argv[])
{
    char option;

    do
    {
        cout << "Menu: " << endl
             << "1 - Binarizar arquivo .csv" << endl
             << "0 - Sair" << endl
             << "Digite a opcao: ";

        // cin >> option;
        option = '1';

        switch (option)
        {
        case '0':
            break;
        case '1':
            process();
            break;
        default:
            cout << "Opcao invalida" << endl;
            break;
        }
        option = '0';

    } while (option != '0');

    return 0;
}