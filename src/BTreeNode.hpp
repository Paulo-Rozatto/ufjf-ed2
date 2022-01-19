#ifndef B_TREE_NODE
#define B_TREE_NODE

template <class T>
class BTreeNode
{
private:
public:
    int M; // max size
    bool isLeaf;
    int currKeys; // current number of keys
    T *keys;
    BTreeNode **children;

    BTreeNode(int M, bool isLeaf);
    ~BTreeNode();
    BTreeNode *split(int i, BTreeNode *child);
    BTreeNode *insert(T key, int *cont);
    BTreeNode *search(T key, int *cont);
};

#endif