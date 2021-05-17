#include "stdio.h"
#include <iostream>
#include <math.h>

using namespace std;

int main() {
	float p, n;
	printf("p = ");
	scanf("%f", &p);
	printf("n = ");
	scanf("%f", &n);

	int k = 0;
	while (pow(p, k) < n) {
		k++;
	}
	k--;

	printf("min k satifies (p^k < n) is: %d\n", k);
}
