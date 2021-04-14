#include "stdio.h"
#include <iostream>
#include <math.h>

using namespace std;

int simplify(int n, int f) {
	if (n%f!=0) return n;
	return simplify(n/f, f);
}

void factor(int org, int n, int f) {
	if (f>n) return;
	if (n%f == 0 && org != f) {
		printf(" %d", f);
	}
	n = simplify(n, f);
	factor(org, n, f+1);
}

void prime(int n) {
	factor(n, n, 2); 
}

int main() {
	int n;
	printf("n = ");
	scanf("%d", &n);
	printf("prime factors:");
	prime(n);
	printf("\n");
}
