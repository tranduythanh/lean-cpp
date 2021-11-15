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

void swap(int *pX, int *pY)
{
    int z = *pX;
    *pX = *pY;
    *pY = z;
}

void getIndexMinMax(int x[], int n, int i, int *indexMin, int *indexMax)
{
    *indexMin = i;
    *indexMax = n-i-1;
    
    for (int k=i; k<n-i; k++)
    {
        if (x[k] < x[*indexMin])
            *indexMin = k;
        if (x[k] > x[*indexMax])
            *indexMax = k;
    }
}

void selectionSortMinMax(int x[], int n)
{
    int indexMin, indexMax;
    for (int i=0; i<n-i-1; i++)
    {
        getIndexMinMax(x, n, i, &indexMin, &indexMax);
        swap(x[i], x[indexMin]);
        if (indexMax == i)
            indexMax = indexMin;
        swap(x[n-i-1], x[indexMax]);
    }
}

int main()
{
    int x[] = {64, 25, 12, 22, 11, 7, 3, 1, 9};
    int n = sizeof(x) / sizeof(x[0]);
    selectionSortMinMax(x, n);
    cout << "Result: \n";
    printArray(x, n);
    return 0;
}