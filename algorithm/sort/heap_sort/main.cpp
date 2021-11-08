#include "stdio.h"
#include "math.h"
#include <iostream>
#include <string>
using namespace std;

void printIndex(int size)
{
    for (int i = 0; i < size; i++)
        cout << i << "\t";
    cout << endl;
}
void printArray(int arr[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        cout << arr[i] << "\t";
    cout << endl;
}

void printArrayColor(int arr[], int size, int start, int end)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (i >= start && i <= end)
            printf("\x1B[32m%d\033[0m\t", arr[i]);
        else
            printf("%d\t", arr[i]);
    }
    cout << endl;
}

void swap(int *pX, int *pY)
{
    int z = *pX;
    *pX = *pY;
    *pY = z;
}

bool isCorrectElement(int x[], int n, int i)
{
    return (
        (2*i >= n) || // both 2i, 2i+1 not in the array
        ( (2*i == n-1) && (x[i] >= x[2*i]) ) || // only 2i+1 not in the array
        ( (2*i <  n-1) && (x[i] >= x[2*i]) && (x[i] >= x[2*i+1])) // both 2i, 2i+1 in the array
    );
}

int shouldSwapWith(int x[], int n, int i)
{
    if (2*i+1 >n-1)
        return 2*i;
    if (x[2*i+1] > x[2*i])
        return 2*i+1;
    return 2*i;
}

void fixHeapFromIndex(int x[], int n, int i)
{
    int curIndex = i;
    while (true)
    {
        printArrayColor(x, n, curIndex, curIndex);
        if (isCorrectElement(x, n, curIndex))
            return;
        int k = shouldSwapWith(x, n, curIndex);
        swap(x[curIndex], x[k]);
        curIndex = k;
    }
    printArrayColor(x, n, curIndex, curIndex);
}

void buildFullHeap(int x[], int n)
{
    for (int i=n/2+1; i>=0; i--)
    {
        fixHeapFromIndex(x, n, i);
    }
}

void heapSort (int x[], int n)
{
    for (int end=n; end>0; end--)
    {
        buildFullHeap(x, end);
        swap(x[0], x[end-1]);
    }
}

int main()
{
    int arr[] = {3, 17, 81, 25, 62, 7, 23, 9, 43};
    int n = sizeof(arr) / sizeof(arr[0]);
    printIndex(n);
    printArray(arr, n);
    printArray(arr, n);
    heapSort(arr, n);
    cout << "Result: \n";
    printArray(arr, n);
    return 0;
}