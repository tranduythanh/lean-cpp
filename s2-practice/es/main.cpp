#include "stdio.h"
#include "math.h"
#include "assert.h"

int main() {
	float a, b, e, c, d, f;
	printf("parameters a, b, e, c, d, f:\n");
	scanf("%f%f%f%f%f%f", &a, &b, &e, &c, &d, &f);

	float det = a*d - b*c;
	if (det == 0) {
		printf("vo nghiem\n");
		return 0;
	}

	float x1=(e*d-b*f)/det;
	float x2=(a*f-c*e)/det;

	if (x1==x2) {
		printf("x = %f", x1);
		return 0;
	}

	printf("x1 = %f\n", x1);
	printf("x2 = %f\n", x2);
	return 0;
}
