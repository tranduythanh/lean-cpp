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

void printRadixMap(int *map[10], int mapN[])
{
    for (int i=0; i<10; i++) {
        cout << "batch " << i << ":\t"<< endl;
        printArray(map[i], mapN[i]);
    }
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

int nthDigit(int z, int radix)
{
    return int((z / pow(10, radix))) % 10;
}


int max(int x[], int n)
{
    int maxValue = 0;
    for (int i = 0; i < n; i++)
        if (x[i] > maxValue)
            maxValue = x[i];
    return maxValue;
}

// x:       17  3   81  25  62  23  7   43  9
//          |                       |
//          | last digit is 7,      |
//          | so it belongs to      |
//          | batch 7               |
//          └-----------------------└---┐
//                                      ▼
// batch:   0   1   2   3   4   5   6   7   8   9
//                                      |
//                                      | (2 values in x is in batch 7)
//                                      ▼
// count:   0   1   1   3   0   1   0   2   0   1
//                                      |
//                                      | these 2 values will be placed
//                                      | in range [6,8) in "output"
//                                      ▼
// count*:  0   1   2   5   5   6   6   8   8   9
//                                      |
//                                  ┌---┘ by iterating from n->0 on x, we get item 7 first
//                                  |   | then its index will be 8-1=7, then reduce count*
//                                  |   | of batch 8 by 1. 17 comes after that then its
//                                  |   | index will be 7-1=6.
//                                  |   |
//                                  |  (idx=count*[7]-1=8-1=7); count*[7]-- = 7;
//                                  |   |
//                                  |   |
//            (idx=count*[7]-1=7-1=6);  |
//                                  |   |
//                                  ▼   ▼
// output:  81  62  3   23  43  25  17  7   9
//
void countSort(int x[], int n, int radix)
{
    int output[n];
    int i, count[10] = {0};

    // Store count of occurrences in count[]
    for (i = 0; i < n; i++)
        count[nthDigit(x[i], radix)]++;

    cout << "Count:\t";
    printArray(count, 10);

    // Change count[i] so that count[i] now contains actual
    // possible positions of this digit in output[]
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    cout << "Count*:\t";
    printArray(count, 10);

    // Build the output array
    for (i = n - 1; i >= 0; i--)
    {
        int digit = nthDigit(x[i], radix);
        output[count[digit] - 1] = x[i];
        count[digit]--;
    }
    cout << "Output:\t";
    printArray(output, n);
    for (i = 0; i < n; i++)
        x[i] = output[i];
}

void radixSort (int x[], int n)
{
    int maxValue = max(x, n);
    int maxRadix = round(log10(maxValue));

    for (int radix=0; radix < maxRadix; radix++)
        countSort(x, n, radix);
}

int main()
{
    int arr[] = {17, 3, 81, 25, 62, 23, 7, 43, 9};
    int n = sizeof(arr) / sizeof(arr[0]);
    printIndex(n);
    printArray(arr, n);
    radixSort(arr, n);
    cout << "Result: \n";
    printArray(arr, n);
    return 0;
}