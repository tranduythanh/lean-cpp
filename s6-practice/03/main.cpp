#include "stdio.h"
#include "math.h"
#include "assert.h"

int main() {
	int n;
	printf("n = ");
	scanf("%d", &n);
	float s = 0;
	for (int i=1; i<=n; i++) {
		s = s + pow(-1, i+1)/i;
	}
	
	printf("result = %f\n", s);
}
