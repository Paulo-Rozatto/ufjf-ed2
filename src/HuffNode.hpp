#ifndef HUFFNODE_HPP
#define HUFFNODE_HPP

class HuffNode
{
private:
    char key;
    int count;
    HuffNode *left;
    HuffNode *right;

public:
    HuffNode();
    HuffNode(char key);
    ~HuffNode();

    char getKey() { return key; }
    int getCount() { return count; }
    void increaseCount() { count++; }
    void setCount(int count) { this->count = count; }
    HuffNode *getLeft() { return left; }
    void setLeft(HuffNode *node) { this->left = node; }
    HuffNode *getRight() { return right; }
    void setRight(HuffNode *node) { this->right = node; }
};

HuffNode::HuffNode()
{
    this->count = 1;
    this->left = nullptr;
    this->right = nullptr;
}

HuffNode::HuffNode(char key)
{
    this->key = key;
    this->count = 1;
    this->left = nullptr;
    this->right = nullptr;
}
HuffNode::~HuffNode()
{
    if (this->left != nullptr)
    {
        delete this->left;
    }

    if (this->right != nullptr)
    {
        delete this->right;
    }
}

#endif