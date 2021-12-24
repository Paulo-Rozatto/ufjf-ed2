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
    for (int i = begin; i < end; i++)
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
    int i = p, j = q, pivot;
    // int pivot = v[(p + q) / 2]->getUpvote();

    // mediana de tres
    int mid = (p + q) / 2;

    if (v[p]->getUpvote() > v[j - 1]->getUpvote())
        swap(&v[p], &v[j - 1]);

    if (v[mid]->getUpvote() > v[j - 1]->getUpvote())
        swap(&v[mid], &v[j - 1]);

    if (v[p]->getUpvote() > v[mid]->getUpvote())
        swap(&v[p], &v[mid]);

    swap(&v[mid], &v[j - 1]);

    pivot = v[j - 1]->getUpvote();

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

        if (i <= j)
        {
            swap(&v[i], &v[j]);
            *contMov += 1;
            // if (i < q)
            i++;
            if (j > p)
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

void heapSort(Register **v, int begin, int end, int *contMov, int *contComp)
{
    int size = end - begin;

    for (int i = size / 2 - 1 + begin; i >= begin; i--)
        heapify(v, i, end + 1, contMov, contComp);

    while (end > begin)
    {
        swap(v[begin], v[end]);
        heapify(v, begin, end, contMov, contComp);
        *contMov += 1;
        end--;
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

    // int pivot = partition(v, begin, end, contMov, contComp);
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


// Intro sort para a tabela hash

typedef struct {
    int version[3];
    int quant = 0;
}versao;

void swapHash(versao **a, versao **b)
{
    versao *aux = *a;
    *a = *b;
    *b = aux;
}

int partitionHash(versao **v, int p, int q, int *contMov, int *contComp)
{
    int i = p, j = q - 1, pivot;

    // mediana de tres
    int mid = v[(p + q) / 2]->quant;
    if (v[p]->quant > v[j]->quant)
        swapHash(&v[p], &v[j]);
    if (v[mid]->quant > v[j]->quant)
        swapHash(&v[mid], &v[j]);
    if (v[p]->quant > v[mid]->quant)
        swapHash(&v[p], &v[mid]);
    swapHash(&v[mid], &v[j]);

    pivot = v[j]->quant;

    while (true)
    {
        *contComp += 1;
        while (v[i]->quant < pivot)
        {
            i++;
            *contComp += 1;
        }

        *contComp += 1;
        while (v[j]->quant > pivot)
        {
            j--;
            *contComp += 1;
        }

        if (i < j)
        {
            swapHash(&v[i], &v[j]);
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

void quickSortHash(versao **v, int p, int r, int *contMov, int *contComp)
{
    int q;
    if (p < r)
    {
        q = partitionHash(v, p, r, contMov, contComp);
        quickSortHash(v, p, q, contMov, contComp);
        quickSortHash(v, q + 1, r, contMov, contComp);
    }
    cout << v[0]->quant << endl;
}

/*
int partitionHash(versao **v, int p, int q, int *contMov, int *contComp)
{
    int i = p, j = q - 1, pivot;

    // mediana de tres
    int mid = v[(p + q) / 2]->quant;
    if (v[p]->quant > v[j]->quant)
        swapHash(&v[p], &v[j]);
    if (v[mid]->quant > v[j]->quant)
        swapHash(&v[mid], &v[j]);
    if (v[p]->quant > v[mid]->quant)
        swapHash(&v[p], &v[mid]);
    swapHash(&v[mid], &v[j]);

    pivot = v[j]->quant;

    while (true)
    {
        *contComp += 1;
        while (v[i]->quant < pivot)
        {
            i++;
            *contComp += 1;
        }

        *contComp += 1;
        while (v[j]->quant > pivot)
        {
            j--;
            *contComp += 1;
        }

        if (i < j)
        {
            swapHash(&v[i], &v[j]);
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

void insertionSortHash(versao **v, int begin, int end, int *contMov, int *contComp)
{
    for (int i = begin; i < end - 1; i++)
    {
        int j = i + 1;
        versao *pivot = v[j];

        *contComp += 1;
        while (j > 0 && pivot->quant < v[j - 1]->quant)
        {
            v[j] = v[j - 1];
            *contComp += 1;
            *contMov += 1;
            j--;
        }

        v[j] = pivot;
    }
}

void heapifyHash(versao **v, int root, int n, int *contMov, int *contComp)
{

    int child;
    int max = root; // assume que a raiz e o maior valor

    while (root < n)
    {
        child = 2 * root + 1;

        *contComp += 1;
        if (child < n && v[child]->quant > v[child]->quant)
        {
            max = child;
            *contComp += 1;
        }

        child += 1;

        *contComp += 1;
        if (child < n && v[child]->quant > v[child]->quant)
        {
            max = child;
            *contComp += 1;
        }

        if (max != root)
        {
            swapHash(&v[root], &v[max]);
            *contMov += 1;
        }
        else
            break;

        root = max;
    }
}

void heapSortHash(versao **v, int begin, int end, int *contMov, int *contComp)
{
    int size = end - begin;

    for (int i = size / 2 - 1 + begin; i >= begin; i--)
        heapifyHash(v, i, end, contMov, contComp);

    while (end > begin)
    {   
        end--;
        swapHash(&v[begin], &v[end]);
        heapifyHash(v, begin, end, contMov, contComp);
    }
}

void introSortRecHash(versao **v, int begin, int end, int depthLimit, int *contMov, int *contComp)
{
    int size = end - begin;

    if (size < 16)
    {
        insertionSortHash(v, 0, 1000000, contMov, contComp);
        return;
    }

    if (depthLimit == 0)
    {
        heapSortHash(v, begin, end, contMov, contComp);
        return;
    }

    int pivot = partitionHash(v, begin, end, contMov, contComp);
    depthLimit -= 1;

    introSortRecHash(v, begin, pivot, depthLimit, contMov, contComp);
    introSortRecHash(v, pivot + 1, end, depthLimit, contMov, contComp);
}

void introSortHash(versao **v, int begin, int end, int *contMov, int *contComp)
{
    int depthLimit = 2 * log(end - begin);

    introSortRecHash(v, begin, end, depthLimit, contMov, contComp);
}
*/

#endif