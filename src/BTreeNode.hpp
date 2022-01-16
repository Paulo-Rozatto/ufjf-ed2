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

    // public:
    void splitChild(int i, BTreeNode *c);
    BTreeNode(int M);
    BTreeNode(int M, bool isLeaf);
    ~BTreeNode(){};
    // void insert(T key);
    BTreeNode* insert(T key);
    void show();
    bool leaf() { return isLeaf; };
    BTreeNode* insertLocal(T key);

    private:
    BTreeNode* split(T key);


};

#endif