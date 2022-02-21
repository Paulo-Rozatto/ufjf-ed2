#include "HuffNode.hpp"

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