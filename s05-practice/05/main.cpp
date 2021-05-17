#include "stdio.h"
#include "math.h"
#include "assert.h"

float cal(int i) {
	return 1 + 1/pow(i,2);
}

int main() {
	int n;
	printf("integer n = ");
	scanf("%d", &n);
	
	float a = 1.0;
	for (int i=1; i<=n; i++) {
		a*=cal(i);
	}

	printf("result = %f", a);
}
