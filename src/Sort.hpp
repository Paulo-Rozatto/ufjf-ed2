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

void swap(Register **a, Register **b)
{
    Register *aux = *a;
    *a = *b;
    *b = aux;
}

// Insertion Sort

void insertionSort(Register **v, int begin, int end, int *contMov, int *contComp)
{
    for (int i = begin; i < end - 1; i++)
    {
        int j = i + 1;
        Register *pivot = v[j];

        *contComp += 1;
        while (j > 0 && pivot->getUpvote() < v[j - 1]->getUpvote())
        {
            v[j] = v[j - 1];
            *contComp += 1;
            *contMov += 1;
            j--;
        }

        v[j] = pivot;
    }
}

// Quicksort

int partition(Register **v, int p, int q, int *contMov, int *contComp)
{
    int pivot = v[(p + q) / 2]->getUpvote();
    int i = p, j = q - 1;

    while (true)
    {
        *contComp += 1;
        while (v[i]->getUpvote() < pivot)
        {
            i++;
            *contComp += 1;
        }

        *contComp += 1;
        while (v[j]->getUpvote() > pivot)
        {
            j--;
            *contComp += 1;
        }

        if (i < j)
        {
            swap(&v[i], &v[j]);
            *contMov += 1;

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

void quickSort(Register **v, int p, int r, int *contMov, int *contComp)
{
    int q;
    if (p < r)
    {
        q = partition(v, p, r, contMov, contComp);
        quickSort(v, p, q, contMov, contComp);
        quickSort(v, q + 1, r, contMov, contComp);
    }
}

// Heap sort

void heapify(Register **v, int root, int n, int *contMov, int *contComp)
{

    int child;
    int max = root; // assume que a raiz e o maior valor

    while (root < n)
    {
        child = 2 * root + 1;

        *contComp += 1;
        if (child < n && v[child]->getUpvote() > v[root]->getUpvote())
        {
            max = child;
            *contComp += 1;
        }

        child += 1;

        *contComp += 1;
        if (child < n && v[child]->getUpvote() > v[max]->getUpvote())
        {
            max = child;
            *contComp += 1;
        }

        if (max != root)
        {
            swap(v[root], v[max]);
            *contMov += 1;
        }
        else
            break;

        root = max;
    }
}

void heapSort(Register **v, int n, int *contMov, int *contComp)
{
    // constroi heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(v, i, n, contMov, contComp);

    while (n > 0)
    {
        n--;
        swap(v[0], v[n]);
        *contMov += 1;
        heapify(v, 0, n, contMov, contComp);
    }
}

void heapSort(Register **v, int begin, int end, int *contMov, int *contComp)
{
    int size = end - begin;

    for (int i = size / 2 - 1 + begin; i >= begin; i--)
        heapify(v, i, end, contMov, contComp);

    while (end > begin)
    {
        end--;
        swap(v[begin], v[end]);
        heapify(v, begin, end, contMov, contComp);
    }
}

// Intro sort

void introSortRec(Register **v, int begin, int end, int depthLimit, int *contMov, int *contComp)
{
    int size = end - begin;

    if (size < 16)
    {
        insertionSort(v, begin, end, contMov, contComp);
        return;
    }

    if (depthLimit == 0)
    {
        heapSort(v, begin, end, contMov, contComp);
        return;
    }

    int pivot = partition(v, begin, end, contMov, contComp);
    depthLimit -= 1;

    introSortRec(v, begin, pivot, depthLimit, contMov, contComp);
    introSortRec(v, pivot + 1, end, depthLimit, contMov, contComp);
}

void introSort(Register **v, int begin, int end, int *contMov, int *contComp)
{
    int depthLimit = 2 * log(end - begin);

    introSortRec(v, begin, end, depthLimit, contMov, contComp);
}

#endif