#include "stdio.h"
#include <iostream>
#include <math.h>

using namespace std;

float power(float r, float x, int n) {
	if (n==0) return r;
	return power(r*x, x, n-1);
}

int main() {
	int n;
	float x;
	printf("x = ");
	scanf("%f", &x);
	printf("n = ");
	scanf("%d", &n);

	float r=1.0;
	printf("r = %f\n", power(r, x, n));
}
