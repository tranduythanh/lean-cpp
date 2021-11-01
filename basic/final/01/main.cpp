#include "stdio.h"

using namespace std;

void printArray(int* args, int n) {
	printf("[ ");
	for (int i = 0; i < n; i++) {
		printf("%d", args[i]);
		if (i < n-1) {
			printf(", ");
		}
	}
	printf(" ]");
}

void printResult(int* args, int n, long long int prod) {
	printf("\n========= Result ==========\n");
	printf("Array: ");
	printArray(args, n);
	printf("\n");
	if (n>0) {
		printf("Product of all elements: %lld", prod);
	} else {
		printf("Product of all elements: No value because array is empty");
	}
	printf("\n========== End ============\n");
}

int main() {
	int n = 0;
	int args[1000];
	long long int prod = 1;

	printf("Input number of integers: ");
	scanf("%d", &n);

	if (n <= 0) {
		printf("Invalid number of elements. It must be positive integer value!");
		return 1;
	}

	for (int i=0; i<n; i++) {
		printf("number #%d: ", i+1);
		scanf("%d", &args[i]);
		prod *= (long long int)args[i];
	}

	printResult(args, n, prod);
	return 0;
}
