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

    BTreeNode(int M, bool isLeaf);
    ~BTreeNode(){};
    BTreeNode* insert(T key);
    BTreeNode* insertLocal(T key);
    BTreeNode *search(T key);
    void show();
    BTreeNode* split(int i, BTreeNode *child);

    private:
    BTreeNode* split(T key);
};

#endif