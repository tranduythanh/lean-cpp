#include "stdio.h"
#include "math.h"
#include "assert.h"

int sumOddN(int n);
int sumSignN(int n);
int sumSquareN(int n);

int main() {
	int n;
	printf("n = ");
	scanf("%d", &n);
	
	printf("sum odd    = %d\n", sumOddN(n));
	printf("sum sign   = %d\n", sumSignN(n));
	printf("sum square = %d\n", sumSquareN(n));
}

int sumOddN(int n) {
	int sum=0;
	for (int i = 1; i <= n; i++) {
		sum += i%2==0 ? i : 0;
	}
	return sum;
}

int sumSignN(int n) {
	int sum=0;
	for (int i = 1; i <= n; i++) {
		switch (i%4) {
			case 1: case 2:
				sum -= i;
				break;
			default:
				sum += i;
				break;
		}
	}
	return sum;
}

int sumSquareN(int n) {
	int sum=0;
	for (int i = 1; i <= n; i++) {
		sum += i*i;
	}
	return sum;
}