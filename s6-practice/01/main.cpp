#include "stdio.h"
#include "math.h"
#include "assert.h"

int main() {
	int n;
	int r = 0;
	printf("n = ");
	scanf("%d", &n);

	while (n > 0) {
		r = r * 10 + (n % 10);
		n = n/10;
	}

	printf("result = %d\n", r);
}
