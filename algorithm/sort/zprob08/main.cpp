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

void swap(int *pX, int *pY)
{
    int z = *pX;
    *pX = *pY;
    *pY = z;
}

int countPath(int x[], int n)
{
    int count = 0;
    for (int i=0; i<n-1; i++)
        if (x[i] > x[i-1])
            count++;
    count++;
    return count;
}

int main()
{
    int x[] = {3, 1, 8, 2, 6, 7};
    int n = sizeof(x) / sizeof(x[0]);
    int r = countPath(x, n);
    cout << "Result: \n";
    printArray(x, n);
    cout << r << endl;
    return 0;
}