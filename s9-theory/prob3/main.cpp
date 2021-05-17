#include "stdio.h"
typedef int arr[100];

void inputArr(arr, int &);
void show(arr, int);
int indexMin(arr, int, int);
void selectSort(arr, int);
void swap(int&, int&);

int main(){
    arr a;
    int n;
    inputArr(a, n);
    show(a, n);
    selectSort(a, n);
    printf("Result ======\n");
    show(a, n);
}

void inputArr(arr a, int &n){
    printf("Nhap so phan tu: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        printf("a[%d] = ", i);
        scanf("%d", &a[i]);
    }
}

void show(arr a, int n){
    printf("(");
    for (int i = 0; i < n; i++)
    {
        printf("%d", a[i]);
        if (i < n - 1)
            printf(",");
    }
    printf(")\n");
}

void swap(int& a, int& b) {
    int c = a;
    a = b;
    b = c;
}

int indexMin(arr a, int begin, int n) {
    int r = begin;
    for (int i=begin+1; i<n;i++)
        r = a[r]<a[i] ? r : i;
    return r;
}

void selectSort(arr a, int n) {
    for (int i=0; i<n;i++) {
        int index = indexMin(a, i, n);
        if (index > i) swap(a[i], a[index]);
    }
}