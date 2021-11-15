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

typedef struct {
    int start;
    int end;
} part;

typedef struct {
    int i;
    int j;
} mark;

mark correctBySwappingLeftRight(int x[], part p)
{
    int i = p.start; 
    int j = p.end;
    int mid = x[(p.start+p.end)/2]; // Chọn phần tử “giữa” làm mốc
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
    return (mark){i, j};
}

void quickSort (int x[], int n)
{
    int pn=0;
    part parts[2*n];
    
    // first part
    parts[0] = (part){0, n-1};
    pn++;

    
    for (int k=0; k<pn; k++)
    {
        int start = parts[k].start;
        int end = parts[k].end;

        mark marker = correctBySwappingLeftRight(x, parts[k]);
        if (start < marker.j)
        {
            parts[pn] = (part){start, marker.j};
            pn++;
        }
        if (end > marker.i)
        {
            parts[pn] = (part){marker.i, end};
            pn++;
        }
    }
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