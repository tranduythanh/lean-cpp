#include "stdio.h"
#include <iostream>
#include <math.h>

using namespace std;

int main() {
	int a, b;
	printf("a = ");
	scanf("%d", &a);
	printf("b = ");
	scanf("%d", &b);

	int n = max(a, b);
	while (n%a != 0 || n%b != 0) {
		n += max(a, b);
	}

	printf("lcm = %d\n", n);
}
