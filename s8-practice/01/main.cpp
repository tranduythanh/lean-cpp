#include "stdio.h"
#include <iostream>
#include <math.h>

using namespace std;

int add(int s, int n) {
	if (n == 0) return s;
	return add(s+n, n-1);
}

int main() {
	int n;
	printf("n = ");
	scanf("%d", &n);

	int s = 0;
	printf("total: %d\n", add(s, n));
}
