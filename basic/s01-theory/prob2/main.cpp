#include "stdio.h"
int main() {
	float a, b, c, max;
	printf("Input 3 numbers:\n");
	scanf("%f%f%f", &a, &b, &c);

	max = a>b   ? a : b;
	max = max<c ? c : max;

	printf("Max: %f", max);

	return 0;
}
