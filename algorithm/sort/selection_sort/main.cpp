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
    for (i = 0; i < size; i++) {
        if (i == step) {
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

int findMinIndexOfChunk(int arr[], int i, int n) 
{
    int minIndex = i;
    for (int j=i; j<n; j++)
        if (arr[j] < arr[minIndex])
            minIndex = j;
    return minIndex;
}

void selectionSort(int arr[], int n)
{
    int i, minIndex;

    for (i = 0; i < n - 1; i++)
    {
        printStep(arr, i+1, n);
        minIndex = findMinIndexOfChunk(arr, i+1, n);
        if (arr[minIndex] < arr[i])
            swap(&arr[minIndex], &arr[i]);
        printStep(arr, i+1, n);
    }
}

int main()
{
    int arr[] = {64, 25, 12, 22, 11};
    int n = sizeof(arr) / sizeof(arr[0]);
    selectionSort(arr, n);
    cout << "Result: \n";
    printArray(arr, n);
    return 0;
}