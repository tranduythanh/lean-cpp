#include "stdio.h"
#include <iostream>
#include <math.h>

using namespace std;

void reverse(int n) {
	if (n==0) return;
	printf("%d", n%10);
	reverse(n/10);
}

int main() {
	int n;
	printf("n = ");
	scanf("%d", &n);

	printf("reverse = ");
	reverse(n);
	printf("\n");
}
