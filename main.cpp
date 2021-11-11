#include <iostream>
#include <fstream>
#include <math.h>

#define BUFFER_SIZE 102400

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

    for (int i = n - 1; i >= 0; i--)
    {
        numb += (v[i] - '0') * pow(10, n - 1);
    }

    return numb;
}

bool writeID(char *buffer, int &pos, int length, fstream &file)
{
    bool needsNextBuffer = false;

    int size = length - pos;

    if (size < 89)
        needsNextBuffer = true;
    else
        size = 89;

    file.write((buffer + pos), size * sizeof(char));
    pos += 90;

    return needsNextBuffer;
}

bool writeReview(char *buffer, int &pos, int length, fstream &file)
{
    bool needsNextBuffer = false;
    char lookinFor = buffer[pos] == '\"' ? '\"' : ',';
    int i;

    for (i = pos + 1; buffer[i] != lookinFor; i++)
    {
        if (i == length)
        {
            needsNextBuffer = true;
            break;
        }
    }
    if (!needsNextBuffer)
    {
        if (lookinFor == '\"')
            i++;

        file.write(buffer + pos, (i - pos) * sizeof(char));
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
    file << upvote;
    pos = i + 1;
    return needsNextBuffer;
}

bool writeVersion(char *buffer, int &pos, int length, fstream &file)
{
    bool needsNextBuffer = false;
    int i, numb;

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
            file << numb;
            pos = i + 1;
        }
    }

    numb = toInt((buffer + pos), (i - pos));
    file << numb;
    pos = i + 1;

    return needsNextBuffer;
}

bool writeDate(char *buffer, int &pos, int length, fstream &file)
{
    bool needsNextBuffer = false;

    int size = length - pos;

    if (size < 19)
        needsNextBuffer = true;
    else
        size = 19;

    file.write((buffer + pos), size * sizeof(char));
    pos += 20;

    return needsNextBuffer;
}

void process()
{
    fstream csv, bin, out;

    // csv.open("archive/tiktok_app_reviews.csv", ios::in);
    csv.open("archive/test.txt", ios::in);
    out.open("out/out.txt", ios::out);
    // bin.open("archive/bin.bin", ios::in | ios::binary);
    // char t[] = {'b'};

    // bin.seekg(4);
    // // bin.write(t, sizeof(char));
    // bin.read(t, sizeof(char));

    if (!csv)
    {
        cout << "Erro: Arquivo archive/tiktok_app_reviews.csv nao encontrado" << endl;
        csv.close();
        return;
    }

    csv.seekg(0, csv.end);
    int length = csv.tellg();
    csv.seekg(0, csv.beg);

    if (length > BUFFER_SIZE)
        length = BUFFER_SIZE;

    char *buffer = new char[length];

    csv.read(buffer, length);

    // out.write(buffer, 89 * sizeof(char));

    int type = ID, pos = 0;

    writeID(buffer, pos, length, out);
    writeReview(buffer, pos, length, out);
    writeUpVote(buffer, pos, length, out);
    writeVersion(buffer, pos, length, out);
    writeDate(buffer, pos, length, out);

    cout << buffer[pos] << endl;

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