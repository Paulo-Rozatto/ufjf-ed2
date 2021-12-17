#ifndef SORT_HPP
#define SORT_HPP

/*
    O algoritmo de ordenação escolhido foi o Intro Sort (introspective sort)
    A função std::sort implementa uma versão do intro sort
    O Intro Sort é um algoritmo hibrido que utiliza o QucickSort, HeapSort e InsertionSort
    Caso o tamanho do vetor seja pequeno, o InsertionSort é utilizado.
    Do contrário, o quick sort é utilizado e, caso o número de chamadas seja execedida, o algortimo troca
    para HeapSort para evitar o pior caso do QuickSort

    https://www.geeksforgeeks.org/know-your-sorting-algorithm-set-2-introsort-cs-sorting-weapon/
    https://www.geeksforgeeks.org/internal-details-of-stdsort-in-c/
*/

void swap(int &a, int &b)
{
    int aux = a;
    a = b;
    b = aux;
}

// Insertion Sort

void insertionSort(int *v, int begin, int end)
{
    for (int i = begin; i < end - 1; i++)
    {
        int j = i + 1;
        int pivot = v[j];

        while (j > 0 && pivot < v[j - 1])
        {
            v[j] = v[j - 1];
            j--;
        }

        v[j] = pivot;
    }
}

// Quicksort

int partition(int *v, int p, int q)
{
    int pivot = v[(p + q) / 2];
    int i = p, j = q - 1;

    while (true)
    {
        while (v[i] < pivot)
            i++;
        while (v[j] > pivot)
            j--;

        if (i < j)
        {
            swap(v[i], v[j]);

            i++;
            j--;
        }
        else
        {
            break;
        }
    }
    return j;
}

void quickSort(int *v, int p, int r)
{
    int q;
    if (p < r)
    {
        q = partition(v, p, r);
        quickSort(v, p, q);
        quickSort(v, q + 1, r);
    }
}

// Heap sort

void heapify(int *v, int root, int n)
{

    int child;
    int max = root; // assume que a raiz e o maior valor

    while (root < n)
    {
        child = 2 * root + 1;

        if (child < n && v[child] > v[root])
            max = child;

        child += 1;
        if (child < n && v[child] > v[max])
            max = child;

        if (max != root)
            swap(v[root], v[max]);
        else
            break;

        root = max;
    }
}

void heapSort(int *v, int n)
{
    // constroi heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(v, i, n);

    while (n > 0)
    {
        n--;
        swap(v[0], v[n]);
        heapify(v, 0, n);
    }
}

void heapSort(int *v, int begin, int end)
{
    int size = end - begin;

    for (int i = size / 2 - 1 + begin; i >= begin; i--)
        heapify(v, i, end);

    while (end > begin)
    {
        end--;
        swap(v[begin], v[end]);
        heapify(v, begin, end);
    }
}

// Intro sort

void introSortRec(int *v, int begin, int end, int depthLimit)
{
    int size = end - begin;

    if (size < 16)
    {
        insertionSort(v, begin, end);
        return;
    }

    if (depthLimit == 0)
    {
        heapSort(v, begin, end);
        return;
    }

    int pivot = partition(v, begin, end);
    depthLimit -= 1;

    introSortRec(v, begin, pivot, depthLimit);
    introSortRec(v, pivot + 1, end, depthLimit);
}

void introSort(int *v, int begin, int end)
{
    int depthLimit = 2 * log(end - begin);

    introSortRec(v, begin, end, depthLimit);
}

#endif