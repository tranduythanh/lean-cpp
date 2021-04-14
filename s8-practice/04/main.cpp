#include "stdio.h"
#include <iostream>
#include <math.h>

using namespace std;

int gcd(int a, int b) {
	if (a%b==0) return b;
	return gcd(b, a%b);
}

int main() {
	int a, b;
	printf("a = ");
	scanf("%d", &a);
	printf("b = ");
	scanf("%d", &b);

	printf("gcd = %d\n", gcd(max(a, b), min(a, b)));
}
