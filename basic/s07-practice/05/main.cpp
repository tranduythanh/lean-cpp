#include "stdio.h"
#include <iostream>
#include <math.h>

using namespace std;

int main() {
	int n;
	printf("n = ");
	scanf("%d", &n);

	int count = 0;
	printf("Inverse of n: ");
	while (n>0) {
		count++;
		printf("%d", n%10);
		n /= 10;
	}
	printf("\n");
	printf("total digit: %d\n", count);
}
