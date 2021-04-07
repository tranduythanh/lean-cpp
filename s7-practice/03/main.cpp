#include "stdio.h"
#include <iostream>
#include <math.h>

using namespace std;

float fac(int n) {
	float f = 1.0;
	for (int i=1; i<=n; i++)
		f *= i;
	return f;
}

float elem(int x, int n) {
	return pow(x, n)/fac(n);
}

int main() {
	float x, epsilon;
	printf("x = ");
	scanf("%f", &x);
	printf("epsilon = ");
	scanf("%f", &epsilon);

	float r = 1.0;
	int n = 1;
	while (elem(x, n) > epsilon) {
		r += elem(x, n);
		n++;
	}
	printf("e^x = %f\n", r);
}
