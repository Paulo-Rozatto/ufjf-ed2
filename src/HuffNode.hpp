#ifndef HUFFNODE_HPP
#define HUFFNODE_HPP

class HuffNode
{
private:
    char key;
    int count;

public:
    HuffNode(char key);
    ~HuffNode(){};

    char getKey() { return key; }
    int getCount() { return count; }
    void increaseCount() { count++; }
};

HuffNode::HuffNode(char key)
{
    this->key = key;
    this->count = 1;
}


#endif