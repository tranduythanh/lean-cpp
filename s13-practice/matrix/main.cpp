#include "stdio.h"
#include "math.h"
#include <algorithm>
#include <string>

using namespace std;

struct Matrix {
	float arr[2000][2000];
	int   size[2];

	void print() {
		printf("\n");
		for (int i=0; i<this->size[0]; i++) {
			for (int k=0; k<this->size[1]; k++)
				printf("%.2f\t", this->arr[i][k]);
			printf("\n");
		}
		printf("\n");
	}

	Matrix subMatrix(int p, int q) {
		Matrix temp;
		int n = this->size[0]-1;
		temp.size[0] = n;
		temp.size[1] = n;
		int i = 0, j = 0;
		for (int row = 0; row < this->size[0]; row++) {
			if (row == p) continue;
			for (int col = 0; col < this->size[1]; col++) {
				if (col == q) continue;
				temp.arr[i][j] = this->arr[row][col];
				if (j == n-1) j=0;
				else          j++;
			}
			i++;
		}
		return temp;
	}

	float det() {
		if (this->size[0] != this->size[1]) return 0;
		int n = this->size[0];
		float d = 0;
		if (n == 1)
			return this->arr[0][0];
		if (n == 2)
			return (this->arr[0][0] * this->arr[1][1]) - (this->arr[0][1] * this->arr[1][0]);
		int sign = 1;
		for (int i = 0; i < n; i++) {
			Matrix temp = this->subMatrix(0, i);
			d += sign * this->arr[0][i] * temp.det();
			sign = -sign;
		}
		return d;
	}

	Matrix t() {
		Matrix tr;
		tr.size[0]=this->size[1];
		tr.size[1]=this->size[0];
		for (int i = 0; i < this->size[0]; ++i)
			for (int j = 0; j < this->size[1]; ++j) {
				tr.arr[j][i] = this->arr[i][j];
			}
		return tr;
	}

	Matrix adj() {
		Matrix a;
		a.size[0] = this->size[0];
		a.size[1] = this->size[1];
		int n = this->size[0];
		if (this->size[0] == 1)
		{
			a.arr[0][0] = 1;
			return a;
		}
		int s = 1;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				Matrix t = this->subMatrix(i, j);
				s = ((i + j) % 2 == 0) ? 1 : -1;   //sign of adj[j][i] positive if sum of row and column indexes is even.
				a.arr[j][i] = (s) * t.det(); //Interchange rows and columns to get the transpose of the cofactor matrix
			}
		}
		return a;
	}

	Matrix inverse() {
		Matrix inv;
		inv.size[0] = this->size[0];
		inv.size[1] = this->size[1];
		int n = this->size[0];
		float det = this->det();
		if (det == 0) {
			printf("can't find its inverse\n");
			return inv;
		}
		Matrix a = this->adj();
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				inv.arr[i][j] = a.arr[i][j] / det;
		return inv;
	}
};

Matrix inputMatrix() {
	Matrix a;
	printf("Input your set:\n");
	printf("Row size: ");
	scanf("%d", &a.size[0]);
	printf("Col size: ");
	scanf("%d", &a.size[1]);
	for (int i=0; i<a.size[0]; i++)
		for (int k = 0; k < a.size[1]; k++) {
			printf("element [%d][%d]: ", i, k);
			scanf("%f", &a.arr[i][k]);
		}
	return a;
}

int main() {
	Matrix a = inputMatrix();
	a.print();

	printf("det = %f\n", a.det());

	Matrix b = a.t();
	b.print();

	Matrix c = a.inverse();
	c.print();
	return 0;
}
