#include "stdio.h"
#include "math.h"
#include "assert.h"

int fac (int n);

int main() {
	int k, n;
	printf("k = ");
	scanf("%d", &k);
	printf("n = ");
	scanf("%d", &n);
	printf("A^k_n = %d\n", fac(n) / fac(n - k));
}

int fac(int n) {
	int p=1;
	for (int i=1; i<=n; i++) {
		p *= i;
	}
	return p;
}

