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

    // template <T>
    // friend class BTree;

    void splitChild(int i, BTreeNode *c);
    BTreeNode(int M, bool isLeaf);
    ~BTreeNode(){};
    BTreeNode* insert(T key);
    BTreeNode* insertLocal(T key);
    void show();

    private:
    BTreeNode* split(T key);
};

#endif