#include "stdio.h"
#include "math.h"
#include "assert.h"

int fac(int n) {
	int p = 1;
	for (int i=1; i<=n; i++) {
		p *= i;
	}
	return  p;
}

int main() {
	int n;
	float x;
	printf("float x = ");
	scanf("%f", &x);
	printf("integer n = ");
	scanf("%d", &n);


	float a = pow(x, n)/fac(n);

	printf("result = %f", a);
}
