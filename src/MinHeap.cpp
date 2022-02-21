#include "MinHeap.hpp"

void MinHeap::swapNodes(int n1, int n2)
{
    HuffNode *aux = heap[n1];
    heap[n1] = heap[n2];
    heap[n2] = aux;
}

MinHeap::MinHeap()
{
}

MinHeap::~MinHeap()
{
    // for (HuffNode *p : heap)
    //     delete p;

    // heap.clear();
}

int MinHeap::find(char key)
{
    int i;
    for (i = 0; i < heap.size(); i++)
    {
        if (heap[i]->getKey() == key)
        {
            return i;
        }
    }

    return -1;
}

void MinHeap::heapify(int idx)
{
    int smallest = idx;

    while (true)
    {
        if (left(idx) < heap.size() && heap[idx]->getCount() > heap[left(idx)]->getCount())
            smallest = left(idx);

        if (right(idx) < heap.size() && heap[smallest]->getCount() > heap[right(idx)]->getCount())
            smallest = right(idx);

        if (smallest == idx)
            break;

        swapNodes(idx, smallest);
        idx = smallest;
    }
}

void MinHeap::insert(HuffNode *node)
{
    heap.push_back(node);
    int i = heap.size() - 1;

    while (i > 0 && heap[parent(i)]->getCount() > heap[i]->getCount())
    {
        swapNodes(parent(i), i);
        i = parent(i);
    }
}

HuffNode *MinHeap::popMin()
{
    if (heap.size() == 0)
    {
        return NULL;
    }

    HuffNode *min = heap[0];
    int last = heap.size() - 1;

    swapNodes(0, last);

    heap.pop_back();
    heapify(0);

    return min;
}

void MinHeap::show()
{
    for (HuffNode *p : heap)
        cout << p->getCount() << " - " << p->getKey() << endl;

    cout << endl;
}