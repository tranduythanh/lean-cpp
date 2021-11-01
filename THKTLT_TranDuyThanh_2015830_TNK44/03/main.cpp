// Viết chương nhập hai số nguyên dương A, B và một mảng gồm N số nguyên.
// Xuất các phẩn tử của mảng.
// Tìm số đầu tiên chia hết cho cả A và B.

#include "stdio.h"

#define M 1000

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

void printResult(int* args, int n, int result, int index) {
	printf("\n========= Result ==========\n");
	printf("Array: ");
	printArray(args, n);
	printf("\n");
	if (index == -1) {
		printf("No element of array is divisible by both A and B");
	} else {
		printf("First element that is divisible by both A and B is %d at index %d", result, index);
	}
	printf("\n========== End ============\n");
}

int mustGetPositiveInteger(char* intro)
{
	int x = 0;
	printf("%s", intro);
	scanf("%d", &x);
	while (x <= 0) {
		printf("You must input a positive integer!\n");
		printf("%s", intro);
		scanf("%d", &x);
	}
	return x;
}

int main() {
	int a, b, n = 0;
	int args[1000];
	int result = 0;
	int index = -1;

	a = mustGetPositiveInteger((char *)"Input the positive integer A: ");
	b = mustGetPositiveInteger((char *)"Input the positive integer B: ");
	n = mustGetPositiveInteger((char *)"Input length of array N:      ");

	for (int i=0; i<n; i++) {
		printf("Element #%d: ", i+1);
		scanf("%d", &args[i]);
		if (index == -1 && args[i] % a == 0 && args[i] % b == 0) {
			result = args[i];
			index = i;
		}
	}

	printResult(args, n, result, index);
	return 0;
}
