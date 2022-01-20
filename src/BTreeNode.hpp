#ifndef B_TREE_NODE
#define B_TREE_NODE

template <class T>
class BTreeNode
{
private:
public:
    int M; // Tamanho maximo
    bool isLeaf;
    int currKeys; // current number of keys
    T *keys;
    BTreeNode **children;

    BTreeNode(int M, bool isLeaf);
    ~BTreeNode();
    void *split(int i, BTreeNode *child);
    void *insert(T key, int *cont);
    BTreeNode *search(T key, int *cont);
};

#endif