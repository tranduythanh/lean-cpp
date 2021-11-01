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

void insertSort(int arr[], int n)
{
    for (int i=n-2; i>=0; i--)
    {
        // get a value from unsorted array
        arr[n] = arr[i];
        int j = i+1;
        
        // find a proper index for it in the sorted array
        while (arr[j] < arr[n])
        {
            arr[j-1] = arr[j];
            j++;
        }
        
        // insert value to sorted array
        if (j>i+1)
            arr[j-1] = arr[n];
        printStep(arr, i, n);
    }
    return;
}

int main()
{
    int arr[] = {60, 21, 19, 22, 11, 17, 1, 90, 56, 43, 3, 27, 33};
    int n = sizeof(arr) / sizeof(arr[0]);
    insertSort(arr, n);
    cout << "Result: \n";
    printArray(arr, n);
    return 0;
}