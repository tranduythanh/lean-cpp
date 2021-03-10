#include "stdio.h"
#include "math.h"
#include "assert.h"


// ax + b = 0
float ptb1(float a, float b) {
	return -b/a;
}

float delta(float a, float b, float c) {
	return b*b - 4*a*c;
}

int main() {
	float a, b, c;
	printf("parameters:\n");
	scanf("%f%f%f", &a, &b, &c);

	if (a == 0) {
		printf("x = %f", ptb1(b, c));
		return 0;
	}
	
	float d = delta(a, b, c);
	if (d < 0) {
		printf("vo nghiem\n");
		return 0;
	}

	if (d==0) {
		printf("x = %f", -b/(2*a) );
		return 0;
	}

	float x1 = -b-sqrt(d)/(2*a);
	float x2 = -b+sqrt(d)/(2*a);
	printf("x1 = %f\n", x1);
	printf("x2 = %f\n", x2);
	return 0;
}
