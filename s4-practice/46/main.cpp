#include "stdio.h"
#include "math.h"
#include "assert.h"

int main() {
	int n;
	float x;
	printf("x = ");
	scanf("%f", &x);
	printf("n = ");
	scanf("%d", &n);

	printf("x^n = %f\n", pow(x, n));
}
