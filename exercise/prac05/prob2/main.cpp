// Viết chương trình thực hiện sắp xếp một dãy gồm N (N ≤ 100) số nguyên với các
// thuật toán sắp xếp đã học: Selection Sort, Quick sort, Merge sort, Shell sort và Heap
// sort. Trong đó, yêu cầu cụ thể như sau:
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

void printArrayColor(int arr[], int size, int start, int end)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (i >= start && i <= end)
            printf("\x1B[32m%d\033[0m\t", arr[i]);
        else
            printf("%d\t", arr[i]);
    }
    cout << endl;
}

void printStep(int arr[], int step, int size)
{
    int i;
    cout << "[ ";
    for (i = 0; i < size; i++)
        if (i == step)
            cout << "]     [ ";
        else
            cout << arr[i] << " ";
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
    for (int j = i; j < n; j++)
        if (arr[j] < arr[minIndex])
            minIndex = j;
    return minIndex;
}

void selectionSort(int arr[], int n)
{
    int i, minIndex;

    for (i = 0; i < n - 1; i++)
    {
        printStep(arr, i + 1, n);
        minIndex = findMinIndexOfChunk(arr, i + 1, n);
        if (arr[minIndex] < arr[i])
            swap(&arr[minIndex], &arr[i]);
        printStep(arr, i + 1, n);
    }
}

void partialize(int x[], int left, int right)
{
    // L, r : lần lượt là chỉ số trái và phải của dãy con của mảng x cần phân hoạch
    int i = left;
    int j = right;
    int mid = abs(x[(left + right) / 2]); // Chọn phần tử “giữa” làm mốc

    do
    {
        while (mid > abs(x[i]))
            i = i + 1;
        while (mid < abs(x[j]))
            j = j - 1;
        if (i <= j)
        {
            swap(&x[i], &x[j]);
            i++;
            j--;
        }
    } while (i <= j);
    if (left < j)
        partialize(x, left, j);
    if (right > i)
        partialize(x, i, right);
    return;
}

void quickSort(int x[], int n)
{
    partialize(x, 0, n - 1);
    return;
}

bool isCorrectElement(int x[], int n, int i)
{
    return (
        (2 * i >= n) ||                                                   // both 2i, 2i+1 not in the array
        ((2 * i == n - 1) && (x[i] <= x[2 * i])) ||                       // only 2i+1 not in the array
        ((2 * i < n - 1) && (x[i] <= x[2 * i]) && (x[i] <= x[2 * i + 1])) // both 2i, 2i+1 in the array
    );
}

int shouldSwapWith(int x[], int n, int i)
{
    if (2 * i + 1 > n - 1)
        return 2 * i;
    if (x[2 * i + 1] < x[2 * i])
        return 2 * i + 1;
    return 2 * i;
}

void fixHeapFromIndex(int x[], int n, int i)
{
    int curIndex = i;
    while (true)
    {
        printArrayColor(x, n, curIndex, curIndex);
        if (isCorrectElement(x, n, curIndex))
            return;
        int k = shouldSwapWith(x, n, curIndex);
        swap(x[curIndex], x[k]);
        curIndex = k;
    }
    printArrayColor(x, n, curIndex, curIndex);
}

void buildFullHeap(int x[], int n)
{
    for (int i = n / 2 + 1; i >= 0; i--)
    {
        fixHeapFromIndex(x, n, i);
    }
}

void heapSort(int x[], int n)
{
    for (int end = n; end > 0; end--)
    {
        buildFullHeap(x, end);
        swap(x[0], x[end - 1]);
    }
}

int main()
{
    // a) Selection Sort: Số âm tăng dần về cuối dãy, số không âm giảm dần về đầu dãy.
    int arr1[] = {3, 17, 81, 25, -1, 62, -2, 7, 23, 9, 43, -3, -18, -79, -5};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    printIndex(n1);
    printArray(arr1, n1);
    selectionSort(arr1, n1);
    cout << "Result: \n";
    printArray(arr1, n1);

    // // b) Quick Sort: Sắp tăng dần theo trị tuyệt đối.
    // int arr2[] = {3, 17, 81, 25, 62, 7, 23, 9, 43, -1, -3, -2, -18, -79, -5};
    // int n2 = sizeof(arr2) / sizeof(arr2[0]);
    // printf("\n\n\n");
    // printIndex(n2);
    // printArray(arr2, n2);
    // quickSort(arr2, n2);
    // cout << "Result: \n";
    // printArray(arr2, n2);

    // // c) Merge Sort: Trộn theo cả 2 cách (tự nhiên và trực tiếp). Sắp tăng dần.
    // // already works like this

    // // d) Shell Sort: Sắp tăng dần với bước chạy tùy chọn (nên lấy theo sách).
    // // already works like this

    // // e) Heap Sort: Sắp giảm dần.
    // int arr3[] = {3, 17, 81, 25, 62, 7, 23, 9, 43, -1, -3, -2, -18, -79, -5};
    // int n3 = sizeof(arr3) / sizeof(arr3[0]);
    // printf("\n\n\n");
    // printIndex(n3);
    // printArray(arr3, n3);
    // heapSort(arr3, n3);
    // cout << "Result: \n";
    // printArray(arr3, n3);

    // heapSort(arr, n);
    return 0;
}