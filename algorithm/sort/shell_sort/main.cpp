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

void shellSort(int arr[], int n)
{
    int maxStep = round(log2(n))-1;
    int i, j, tmp;
    for (int step = maxStep; step>0; step--)
    {
        for (i = step; i < n; i++)
        {
            tmp = arr[i];
            j = i - step;
            while (j >= 0 && tmp < arr[j])
            {
                arr[j + step] = arr[j];
                j = j - step;
            }
            arr[j + step] = tmp;
        }
    }
    return;
}

int main()
{
    int arr[] = {3, 17, 81, 25, 62, 7, 23, 9, 43};
    int n = sizeof(arr) / sizeof(arr[0]);
    shellSort(arr, n);
    cout << "Result: \n";
    printArray(arr, n);
    return 0;
}