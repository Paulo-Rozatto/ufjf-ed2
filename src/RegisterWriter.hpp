#ifndef REGISTER_H

#include <fstream>
#include <queue>

using namespace std;

class RegisterWriter
{
private:
    RegisterWriter(){};
    ~RegisterWriter(){};

    static int toInt(char *v, int n);

public:
    static bool writeID(char *buffer, int &pos, int length, fstream &file);
    static void writeReview(queue<string> &q, fstream &file);
    static bool reviewToQueue(char *buffer, int &pos, int length, queue<string> &q, fstream &file, int &filePos);
    static bool writeUpVote(char *buffer, int &pos, int length, fstream &file);
    static bool writeVersion(char *buffer, int &pos, int length, fstream &file);
    static bool writeDate(char *buffer, int &pos, int length, fstream &file);
};
#endif