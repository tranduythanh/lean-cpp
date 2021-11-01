#include "stdio.h"
#include "math.h"
#include <iostream>
#include <string>
using namespace std;

void printArray(int arr[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        cout << arr[i] << " ";
    cout << endl;
}

void printArrayColor(int arr[], int size, int start, int end)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (i >= start && i <= end)
            printf("\x1B[32m%d\033[0m ", arr[i]);
        else
            printf("%d ", arr[i]);
    }
    cout << endl;
}

void swap(int *pX, int *pY)
{
    int z = *pX;
    *pX = *pY;
    *pY = z;
}

void printStep(int arr[], int step, int size)
{
    int i;
    cout << "[ ";
    for (i = 0; i < size; i++)
    {
        if (i == step)
        {
            cout << "]     [ ";
        }
        cout << arr[i] << " ";
    }
    cout << "]";
    cout << endl;
}

void bubleSort(int arr[], int n)
{
    int last, pairNo = n-1;
    while (pairNo > 0)
    {
        last = 0;
        for (int i=0; i<pairNo; i++)
        {
            printArrayColor(arr, n, i, i+1);
            if (arr[i] > arr[i+1])
            {
                swap(&arr[i], &arr[i+1]);
                last = i;
            }
            printArrayColor(arr, n, i, i + 1);
        }
        printStep(arr, last, n);
        pairNo = last;
    }
    return;
}

int main()
{
    int arr[] = {3, 1, 8, 2, 6, 7};
    int n = sizeof(arr) / sizeof(arr[0]);
    bubleSort(arr, n);
    cout << "Result: \n";
    printArray(arr, n);
    return 0;
}