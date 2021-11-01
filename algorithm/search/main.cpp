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

int linearSearch(int arr[], int n, int elem)
{
    int i = 0;
    while (i < n && arr[i] != elem)
        i += 1;
    if (i >= n)
        return 0;
    return i+1;
}

int linearSearchGuard(int arr[], int n, int elem)
{
    int i = 0;
    arr[n] = elem;
    while (arr[i] != elem)
        i += 1;
    if (i == n)
        return 0;
    return i+1;
}

int binarySearch(int sortedArr[], int n, int elem)
{
    int start = 0;
    int end = n-1;
    int mid = 0;
    while (start < end)
    {
        mid = (start + end)/2;
        if (elem == sortedArr[mid])
            break;
        if (elem < sortedArr[mid])
            end = mid - 1;
        else
            start = mid + 1;
    }
    if (start <= end)
        return mid+1;
    return 0;
}

int main()
{
    int arr[] = {64, 25, 12, 22, 11};
    int n = sizeof(arr) / sizeof(arr[0]);
    int elem = 22;
    
    cout << endl << "linearSearch =================" << endl;
    int index = linearSearch(arr, n, elem);
    printArray(arr, n);
    cout << "Element " << elem << " is at index " << index << " (index from 1)" << endl;

    cout << endl << "linearSearchGuard ============" << endl;
    index = linearSearchGuard(arr, n, elem);
    printArray(arr, n);
    cout << "Element " << elem << " is at index " << index << " (index from 1)" << endl;

    int sortedArr[] = {3, 5, 6, 10, 17, 22, 23, 50, 90};
    n = sizeof(sortedArr) / sizeof(sortedArr[0]);
    cout << endl << "binarySearch =================" << endl;
    index = linearSearchGuard(sortedArr, n, elem);
    printArray(sortedArr, n);
    cout << "Element " << elem << " is at index " << index << " (index from 1)" << endl;
}
