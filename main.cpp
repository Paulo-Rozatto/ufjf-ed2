#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <queue>
#include <chrono>
#include <time.h>

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

class Register
{
private:
    char id[90];
    int upvote;
    int version[3];
    char date[20];
    char *review;

public:
    Register();
    Register(fstream &file, int index);
    ~Register();

    void init(fstream &file, int index);
    void print();
    void print(fstream &file);
};

void Register::init(fstream &file, int i)
{
    // id (89 char) + review position (1 int) + upvote (1 int) + version (3 int) + date (19 char)
    const int ROW_SIZE = 89 * sizeof(char) + sizeof(int) + sizeof(int) + 3 * sizeof(int) + 19 * sizeof(char);
    int aux, reviewPosition, size;

    i *= ROW_SIZE;

    file.seekg(i, file.beg);

    file.read(id, 89 * sizeof(char));
    file.read(reinterpret_cast<char *>(&reviewPosition), sizeof(reviewPosition));
    file.read(reinterpret_cast<char *>(&upvote), sizeof(upvote));
    file.read(reinterpret_cast<char *>(version), sizeof(version));
    file.read(date, 19 * sizeof(char));

    file.seekg(reviewPosition, file.beg);
    file.read(reinterpret_cast<char *>(&size), sizeof(size));

    if (review != nullptr)
        delete[] review;

    review = new char[size + 1];

    file.read(review, size * sizeof(char));

    id[89] = date[19] = review[size] = '\0';
}

void Register::print()
{
    if (review != nullptr)
    {
        cout << "id: " << id << endl
             << "upvote: " << upvote << endl;

        if (version[0] == 0 && version[1] == 0 && version[2] == 0)
            cout << "version: "
                 << "not informed" << endl;
        else
            cout << "version: " << version[0] << "." << version[1] << "." << version[2] << endl;

        cout << "date: " << date << endl
             << "review: " << review << endl;
    }
}

void Register::print(fstream &file)
{
    if (review != nullptr)
    {
        file << "id: " << id << endl
             << "upvote: " << upvote << endl;

        if (version[0] == 0 && version[1] == 0 && version[2] == 0)
            file << "version: "
                 << "not informed" << endl;
        else
            file << "version: " << version[0] << "." << version[1] << "." << version[2] << endl;

        file << "date: " << date << endl
             << "review: " << review << endl;
    }
}

Register::Register()
{
    review = nullptr;
}

Register::Register(fstream &file, int index)
{
    review = nullptr;
    init(file, index);
}

Register::~Register()
{
    if (review != nullptr)
        delete[] review;
}

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

bool reviewToQueue(char *buffer, int &pos, int length, queue<string> &q, fstream &file, int &filePos)
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
            needsNextBuffer = writeID(buffer, pos, bufferLength, out);
            if (needsNextBuffer)
                break;

            next = REVIEW;
        }

        case REVIEW:
        {
            needsNextBuffer = reviewToQueue(buffer, pos, bufferLength, reviewQueue, out, rowPos);
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
            readFileToBuffer(buffer, bufferLength, csv, filePos, FILE_SIZE);
            pos = 0;
        }
    }

    writeReview(reviewQueue, out);

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

    Register r;
    int j;

    if (c == '1')
    {
        for (int i = 0; i < 10; i++)
        {
            j = rand() % ROWS;

            cout << (i + 1) << " - Registro " << j << ":" << endl;
            r.init(bin, j);
            r.print();
            cout << endl;
        }
    }
    else
    {
        fstream out;
        out.open("teste_importacao.txt", ios::out);

        for (int i = 0; i < 100; i++)
        {
            j = rand() % ROWS;

            out << (i + 1) << " - Registro " << j << ":" << endl;
            r.init(bin, j);
            r.print(out);
            out << endl;
        }
    }
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
        default:
            cout << "Opcao invalida" << endl;
            break;
        }

    } while (option != '0');

    return 0;
}