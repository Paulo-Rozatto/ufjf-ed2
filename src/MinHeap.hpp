#ifndef MIN_HEAP_HPP
#define MIN_HEAP_HPP

#include "HuffNode.hpp"
#include <vector>
#include <iostream>

using namespace std;

class MinHeap
{
private:
    vector<HuffNode *> heap;

    int find(char key);
    void heapify(int idx);
    void swapNodes(int n1, int n2);

public:
    MinHeap();
    ~MinHeap();

    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }

    void insert(HuffNode *node);
    HuffNode *popMin();

    HuffNode *getRoot() { return heap[0]; }
    int getSize() { return heap.size(); }
    void show();
};

#endif