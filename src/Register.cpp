#include "Register.hpp"
#include <iostream>

using namespace std;

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