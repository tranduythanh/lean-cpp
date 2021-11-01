#include "stdio.h"
#include <iostream>
#include <math.h>

using namespace std;

void dec2bin(int n) {
	if (n==0) return;
	dec2bin(n/2);
	printf("%d", n%2);
}

int main() {
	int n;
	printf("dec = ");
	scanf("%d", &n);

	printf("bin = ");
	dec2bin(n);
	printf("\n");
}
