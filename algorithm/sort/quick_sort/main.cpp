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

void partialize(int x[], int left, int right)
{
    // L, r : lần lượt là chỉ số trái và phải của dãy con của mảng x cần phân hoạch
    int i = left; 
    int j = right;
    int mid = x[(left+right)/2]; // Chọn phần tử “giữa” làm mốc
    
    do
    {
        while (mid>x[i]) i = i+1;
        while (mid<x[j]) j = j-1;
        if (i <= j)
        {
            swap(&x[i],&x[j]);
            i++ ;
            j-- ;
        }
    } while (i <= j);
    if (left < j)
        partialize(x, left, j);
    if (right > i)
    partialize(x, i, right);
    return;
}

void quickSort (int x[], int n)
{
    partialize(x, 0, n-1);
    return;    
}

int main()
{
    int arr[] = {3, 17, 81, 25, 62, 7, 23, 9, 43};
    int n = sizeof(arr) / sizeof(arr[0]);
    quickSort(arr, n);
    cout << "Result: \n";
    printArray(arr, n);
    return 0;
}