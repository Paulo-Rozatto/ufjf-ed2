#ifndef B_KEY_HPP
#define B_KEY_HPP

#include <string.h>
#include <iostream>

class BKey
{
private:
    int index;
    char id[89];

public:
    BKey(){};
    BKey(int index, char *id);
    ~BKey();
    int getIndex() { return this->index; };
    char *getId() { return id; };
    void init(int index, char *id);

    friend bool operator==(BKey &b1, BKey &b2);
    friend bool operator>(BKey &b1, BKey &b2);
    friend bool operator<(BKey &b1, BKey &b2);
    friend std::ostream &operator<<(std::ostream &outs, const BKey &key);
};


#endif