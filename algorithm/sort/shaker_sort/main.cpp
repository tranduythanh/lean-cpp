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

void shakerSort(int arr[], int n)
{
    int tmpIndex, j;
    int left = 0;
    int right = n-1;
    do {
        tmpIndex = right;
        for (j=right; j>left; j--)
        {
            if (arr[j] < arr[j-1])
            {
                swap(&arr[j], &arr[j-1]);
                tmpIndex = j;
            }
        }
        left = tmpIndex;

        tmpIndex = left;
        for (j = left; j < right; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(&arr[j], &arr[j + 1]);
                tmpIndex = j;
            }
        }
        right = tmpIndex;
    } while (left < right);
    
    return;
}

int main()
{
    int arr[] = {3, 17, 81, 25, 62, 7, 23, 9, 43};
    int n = sizeof(arr) / sizeof(arr[0]);
    shakerSort(arr, n);
    cout << "Result: \n";
    printArray(arr, n);
    return 0;
}