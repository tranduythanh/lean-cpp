#include "stdio.h"
#include "math.h"
#include "assert.h"
#include "map"
#include <bitset>
#include <iostream>

#define M 100

using namespace std;

bitset<M> encode(int x[], int nx) {
	bitset<M> bs;
	for (int i=0; i<nx; i++) {
		bs.set(x[i], true);
	}
	return bs;
}

void decode(bitset<M> bs, int* r) {
	int i = 0;
	int size = bs.size();
	for (int pos=0; pos<size; pos++) {
		if (bs.test(pos)) {
			r[i] = pos;
			i++;
		}
	}
}

void printArr(int x[], int nx) {
	for (int i=0; i<nx; i++) {
		printf("\t%d", x[i]);
	}
	cout << endl;
}

void showBitset(bitset<M> bs) {
	cout << bs  << endl;
	int l = bs.count();
	int r[l];
	decode(bs, r);
	printArr(r, l);
}

void calculateAnd(bitset<M> bx, bitset<M> by) {
	bitset<M> bs = bx & by;
	showBitset(bs);
}

void calculateOr(bitset<M> bx, bitset<M> by) {
	bitset<M> bs = bx | by;
	showBitset(bs);
}

void calculateSubtract(bitset<M> bx, bitset<M> by) {
	bitset<M> bs = bx & (~by);
	showBitset(bs);
}

int main() {
	int x[] = {0, 1, 2, 5, 7};
	int y[] = {2, 3, 4, 5};

	int nx = sizeof(x)/sizeof(x[0]);
	int ny = sizeof(y)/sizeof(y[0]);
	
	bitset<M> bx = encode(x, nx);
	bitset<M> by = encode(y, ny);


	cout << bx << endl;
	cout << by << endl;

	calculateAnd(bx, by);
	calculateOr(bx, by);
	calculateSubtract(bx, by);
	return 0;
}
