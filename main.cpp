#include <iostream>
#include <fstream>
#include <math.h>
#include <queue>

// #define BUFFER_SIZE 2048000
#define BUFFER_SIZE 102400
#define ROWS 3646476
// #define ROWS 400

using namespace std;

enum
{
    ID,
    REVIEW,
    UPVOTE,
    VERSION,
    DATE
};

struct review
{
    int size;
    string text;
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

    if (size <= 89)
    {
        needsNextBuffer = true;
    }
    else
    {
        size = 89;

        file.write((buffer + pos), size * sizeof(char));
        pos += size + 1;
        cont++;
    }

    return needsNextBuffer;
}

/* 
    Objetivo: jogar o review para um segundo buffer mantendo a estrura no arquivo binario
    - func do buffer comun para o buffer review que informe em que posicao no arquivo o review ficara
    - escrever buffer review no arquivo
*/

void writeReview(queue<review *> &q, fstream &file)
{
    review *r;
    file.seekp(0, file.end);
    while (!q.empty())
    {
        r = q.front();
        file.write(reinterpret_cast<const char *>(&r->size), sizeof(int));
        file.write(r->text.c_str(), r->text.size() * sizeof(char));

        q.pop();
        delete r;
    }
}

bool reviewToBuffer(char *buffer, int &pos, int length, queue<review *> &q, fstream &file, int &filePos)
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
        review *r = new review();
        // escreve enderco de onde esta o review
        file.write(reinterpret_cast<const char *>(&filePos), sizeof(filePos));

        // calcula tamanho do comentario e salva o tamanho no arquivo
        size = i - pos;

        // r->size = size;
        r->size = text.size();
        r->text = text;

        q.push(r);

        // calcula a posicao para o proximo review
        filePos += text.size() * sizeof(char) + sizeof(int);

        pos = i + 1;
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
        for (i = pos; buffer[i] != ','; i++)
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
        // escreve enderco de onde esta o review
        file.write(reinterpret_cast<const char *>(&row), sizeof(row));

        // salva o cursor atual do arquivo
        int aux = file.tellp();

        // avanca no arquivo ate o local onde vai se colocar o comentario
        file.seekp(row, file.beg);

        // calcula tamanho do comentario e salva o tamanho no arquivo
        size = i - pos;
        file.write(reinterpret_cast<const char *>(&size), sizeof(size));

        // escreve o comentario no arquivo
        file.write(buffer + pos, size * sizeof(char));

        // calcula a posicao para o proximo review
        row += size + sizeof(size);

        // volta o cursor do arquivo para o valor salvo
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

void readToBuffer(char *buffer, int &length, fstream &file, int &pos, int fileSize)
{
    length = fileSize - pos;
    file.seekg(pos, file.beg);

    if (length > BUFFER_SIZE)
        length = BUFFER_SIZE;

    file.seekg(pos);
    file.read(buffer, length);
}

void process()
{
    fstream csv, out;

    // csv.open("archive/test.txt", ios::in);
    csv.open("archive/tiktok_app_reviews.csv", ios::in);
    out.open("archive/tiktok_app_reviews.bin", ios::out | ios::binary);

    if (!csv.is_open())
    {
        cout << "Erro: Arquivo archive/tiktok_app_reviews.csv nao encontrado" << endl;
        csv.close();
        out.close();
        return;
    }

    if (!out.is_open())
    {
        cout << "Erro: Nao foi possivel ler/criar arquivo archive/tiktok_app_reviews.bin" << endl;
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
    queue<review *> reviewQueue;

    while (pos < bufferLength)
    {
        switch (next)
        {
        case ID:
        {
            needsNextBuffer = writeID(buffer, pos, bufferLength, out);
            if (needsNextBuffer)
                break;

            next = REVIEW;
        }

        case REVIEW:
        {
            // needsNextBuffer = writeReview(buffer, pos, bufferLength, out, rowPos);
            needsNextBuffer = reviewToBuffer(buffer, pos, bufferLength, reviewQueue, out, rowPos);
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
            // cout << filePos / 543022558.0 << endl; // porcentagem do arquivo processado
            readToBuffer(buffer, bufferLength, csv, filePos, FILE_SIZE);
            pos = 0;
        }
    }

    writeReview(reviewQueue, out);

    csv.close();
    out.close();
    delete[] buffer;
}

void test()
{
    fstream bin;
    // id (89 char) + review position (1 int) + upvote (1 int) + version (3 int) + date (19 char)
    const int ROW_SIZE = 89 * sizeof(char) + sizeof(int) + sizeof(int) + 3 * sizeof(int) + 19 * sizeof(char);

    bin.open("archive/tiktok_app_reviews.bin", ios::in | ios::binary);
    char id[89], date[19];
    int position;
    int upvote, version[3], size;
    int i = 3646475, aux;
    // i = 0;
    i *= ROW_SIZE;

    bin.seekg(i, bin.beg);
    aux = bin.tellg();

    bin.read(id, 89 * sizeof(char));
    bin.read(reinterpret_cast<char *>(&position), sizeof(position));
    bin.read(reinterpret_cast<char *>(&upvote), sizeof(upvote));
    bin.read(reinterpret_cast<char *>(version), sizeof(version));
    bin.read(date, 19 * sizeof(char));

    // position = 53096;
    bin.seekg(position, bin.beg);
    bin.read(reinterpret_cast<char *>(&size), sizeof(size));

    char *review = new char[size];

    bin.read(review, size * sizeof(char));
    bin.close();
    delete[] review;
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
            // test();
            break;
        default:
            cout << "Opcao invalida" << endl;
            break;
        }
        option = '0';

    } while (option != '0');

    return 0;
}