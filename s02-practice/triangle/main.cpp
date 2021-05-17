#include "stdio.h"
#include "math.h"
#include "assert.h"

bool isRight(float a, float b, float c) {
	return a*a+b*b==c*c || b*b+c*c==a*a || c*c+a*a==b*b;
}

bool isIsosceles(float a, float b, float c) {
	return a==b || b==c || c==a;
}

bool isEquilateral(float a, float b, float c) {
	return a==b && b==c;
}

bool isTriangle(float a, float b, float c) {
	return a+b>c && b+c>a && c+a>b;
}

void test() {
	assert(isRight(3,4,5));
	assert(isIsosceles(3,3,4));
	assert(isIsosceles(3,4,4));
	assert(isIsosceles(4,3,4));
	assert(isEquilateral(5,5,5));
	assert(isTriangle(1,1,2)==false);
}

int main() {
	test();
	float a, b, c;
	printf("3 edges:\n");
	scanf("%f%f%f", &a, &b, &c);

	if (! isTriangle(a,b,c)) {
		printf("invalid edges\n");
		return 0;
	}

	if (isEquilateral(a,b,c)) {
		printf("equilateral\n");
		return 0;
	}

	if (isIsosceles(a,b,c)) printf("isosceles\n");
	if (isRight(a,b,c))     printf("right\n");

	return 0;
}
