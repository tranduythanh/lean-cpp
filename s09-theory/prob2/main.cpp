#include "stdio.h"
typedef int matrix[50][50];

void inputMatrix(matrix, int&, int&);
void show(matrix, int, int);
void matMul(matrix, int, int, matrix, int, int, matrix);

int main(){
    matrix a, b, c;
    int am, an, bm, bn;
    inputMatrix(a, am, an);
    show(a, am, an);
    inputMatrix(b, bm, bn);
    show(b, bm, bn);

    printf("--------------------\n");
    matMul(a, am, an, b, bm, bn, c);
    show(c, am, bn);
    return 0;
}

void inputMatrix(matrix a, int& m, int& n) {
    printf("number of rows   : ");
    scanf("%d", &m);
    printf("number of columns: ");
    scanf("%d", &n);
    for (int i=0; i<m; i++) 
        for (int k=0; k<n; k++) {
            printf("A[%d][%d]: ", i, k);
            scanf("%d", &a[i][k]);
        }
}

void show(matrix a, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int k = 0; k < n; k++) {
            printf("\t%d", a[i][k]);
        }
        printf("\n");
    }
}

void matMul(matrix a, int am, int an, matrix b, int bm, int bn, matrix c) {
    for (int i=0; i<am; i++)
        for (int k=0; k<bn; k++) {
            c[i][k]=0;
            for (int t=0; t<an; t++)
                c[i][k] += a[i][t]*b[t][k];
        }
}