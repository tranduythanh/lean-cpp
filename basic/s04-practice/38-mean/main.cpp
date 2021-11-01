#include "stdio.h"
#include "math.h"
#include "assert.h"

int main() {
	float sum = 0;
	for (int i = 0; i < 10; i++){
		float a;
		printf("input number #%d: ", i+1);
		scanf("%f", &a);
		sum += a;
	}
	printf("mean = %f\n", sum/10);
}
