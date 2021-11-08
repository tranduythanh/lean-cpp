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

void printArrayColor(int arr[], int n, int start, int end)
{
    int right = start + ceil((end - start) / 2.0);
    string green = "\x1B[32m";
    string red   = "\x1B[31m";
    string clear = "\033[0m";

    
    for (int i = 0; i < n; i++)
    {
        if (i >= start && i < right)
            printf("%s%d%s\t", green.c_str(), arr[i], clear.c_str());
        else if ( i>= right && i < end)
            printf("%s%d%s\t", red.c_str(), arr[i], clear.c_str());
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


void copyTo(int from[], int to[], int start, int end)
{
    for (int i=start; i < end; i++)
        to[i] = from[i];
}

void sortXFromY(int x[], int y[], int n, int start, int end)
{
    printArrayColor(x, n, start, end);

    // x: ...|                                 |...
    //      [t -->                             )
    // y: ...|   left part    |   right part   |...
    //      [j-->            )[k-->            )...
    //
    int right = start + ceil((end - start) / 2.0);
    int t = start; // run on both  part on x [start, end)
    int j = start; // run on left  part on y [start,right)
    int k = right; // run on right part on y [right, end)

    while (j < right || k < end)
    {
        if (j >= right) {
            x[t] = y[k];
            k++;
        } else if (k >= end) {
            x[t] = y[j];
            j++;
        } else {
            if (y[j] < y[k]) {
                x[t] = y[j];
                j++;
            } else {
                x[t] = y[k];
                k++;
            }
        }
        t++;
    }

    printArrayColor(x, n, start, end);
}

void divideToConquer(int x[], int y[], int n, int start, int end)
{
    // split to left-right parts
    // ...|                                     |...
    // [start]                                [end]
    // ...|    left part     |    right part    |...
    // [left-start]      [left-end]
    //                   [right-start]      [right-end]
    //
    int leftStart = start;
    int leftEnd = leftStart + ceil((end-start)/2.0);
    int rightStart = leftEnd;
    int rightEnd = end;

    //  conquer left part
    if (leftEnd-leftStart > 1)
        divideToConquer(x, y, n, leftStart, leftEnd);

    //  conquer right part
    if (rightEnd - rightStart > 1)
        divideToConquer(x, y, n, rightStart, rightEnd);

    copyTo(x, y, start, end);
    sortXFromY(x, y, n, start, end);
}

void mergeSort (int x[], int n)
{
    int y[n];
    divideToConquer(x, y, n, 0, n);
}

int main()
{
    int arr[] = {17, 3, 81, 25, 62, 23, 7, 43, 9};
    int n = sizeof(arr) / sizeof(arr[0]);
    printIndex(n);
    printArray(arr, n);
    mergeSort(arr, n);
    cout << "Result: \n";
    printArray(arr, n);

    return 0;
}