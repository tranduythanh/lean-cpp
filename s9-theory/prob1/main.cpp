#include "stdio.h"
typedef int arr[100];

void inputArr(arr, int&);
void show(arr, int);
void addVector(arr, arr, int, arr);
int scalarProduct(arr, arr, int);

int main(){
    arr v,u,w; int nv,nu;
    inputArr(v, nv);
    show(v, nv);
    inputArr(u, nu);
    show(u, nu);
    addVector(v,u,nu,w);
    printf("Mang w la: ");
    show(w, nu);
}

void inputArr(arr a, int& n){
    printf("Nhap so phan tu: ");
    scanf("%d",&n);
    for (int i=0; i<n; i++){
        printf("a[%d] = ",i);
        scanf("%d",&a[i]);
    }
}

void show(arr a, int n){
    printf("(");
    for (int i=0; i<n; i++){
        printf("%d", a[i]);
        if(i < n-1) printf(",");
    }
    printf(")\n");
}

void addVector(arr a, arr b, int n, arr c){
    for (int i=0; i < n; i++){
        c[i]=a[i]+b[i];
    }
}

int scalarProduct(arr a, arr b, int n){
    int sum = 0;
    for (int i=0; i<n; i++){
        sum = a[i] + b[i];
    }
    return sum;
}