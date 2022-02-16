#ifndef REGISTER_H
#include <fstream>

using namespace std;

class Register
{
private:
    char id[90];
    int upvote;
    int version[3];
    char date[20];
    char *review;
    int index;

public:
    Register();
    Register(fstream &file, int index);
    ~Register();

    void init(fstream &file, int index);
    void initIdOnly(fstream &file, int index);
    void print();
    void print(fstream &file);

    int getUpvote() { return upvote; };
    int *getVersion() { return version; };
    char *getID() { return id; };
    int getIndex() { return index; }
    void setUpvote(int value) { this->upvote = value; }
};
#endif