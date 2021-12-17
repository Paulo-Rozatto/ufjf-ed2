#ifndef SORT_HPP
#define SORT_HPP

void swap(int &a, int &b)
{
    int aux = a;
    a = b;
    b = aux;
}

// Insertion Sort

void insertionSort(int *v, int n)
{
    for(int i = 0; i < n - 1; i++)
    {
        int j = i + 1;
        int pivot = v[j];

        while (j > 0 && pivot > v[j - 1])
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

#endif