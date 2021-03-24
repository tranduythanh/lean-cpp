#include "stdio.h"
#include "math.h"
#include "assert.h"

int calculateY(int x, int n) {
	return (n-x)%2==0? (n-x)/2 : -1;
}

int main() {
	int n;
	printf("integer n = ");
	scanf("%d", &n);

	for(int x=1; x<=n; x++) {
		int y = calculateY(x, n);
		if (y > 0) {
			printf("x = %d\ty = %d\n", x, y);
		}
	}
	printf("Done--------");
}
