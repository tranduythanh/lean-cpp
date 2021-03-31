#include "stdio.h"
#include "math.h"
#include "assert.h"

float fac(int n) {
	float r=1;
	for (int i=1; i<=n; i++) {
		r = r * i;
	}
	return r;
}

float absError(float x, int n) {
	float err =  pow(x, 2*n+1)/fac(2*n+1);
	return abs(err);
}

float elem(float x, int n) {
	return pow(-1, n) * pow(x, 2*n+1)/fac(2*n+1);
}

int main() {
	float x, e;
	printf("x = ");
	scanf("%f", &x);
	printf("e = ");
	scanf("%f", &e);

	int n = 0;
	float sin = 0.0;
	while (absError(x, n) > e && n < 20) {
		sin = sin + elem(x, n);
		printf("debug n=%d\tsin=%f\telem=%f\terr=%f\n", n, sin, elem(x, n), absError(x, n));
		n++;
	}
	printf("result = %f\n", sin);
}
