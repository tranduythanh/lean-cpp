#include "stdio.h"
int main() {
	int a, b;
	printf("Input 2 numbers:\n");
	scanf("%d%d", &a, &b);
	
	printf("Sum:      %d\n", a+b);
	printf("Subtract: %d\n", a-b);
	
	if (b == 0) {
		printf("Cannot devide\n");
	} else {
		printf("Devide:   %.3f\n", float(a)/b );;
		printf("Modulo:   %d %d", a/b, a%b );
	}
	
	return 0;
}
