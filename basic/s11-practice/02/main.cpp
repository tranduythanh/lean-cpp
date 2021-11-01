#include "stdio.h"
#include "math.h"
#include <string>

struct Complex {
	float r;
	float i;
};

Complex getComplex(char *msg) {
	Complex c;
	printf("%s", msg);
	scanf("%f+%fi", &c.r, &c.i);
	return c;
}

void show(Complex c) {
	printf("Complex: %f+%fi\n", c.r, c.i);
}

Complex add(Complex c1, Complex c2) {
	Complex c;
	c.r = c1.r  + c2.r;
	c.i = c1.i + c2.i;
	return c;
}

Complex minus(Complex c1, Complex c2) {
	Complex c;
	c.r = c1.r - c2.r;
	c.i = c1.i - c2.i;
	return c;
}

Complex mul(Complex c1, Complex c2) {
	Complex c;
	c.r = c1.r*c2.r - c1.i*c1.i;
	c.i = c1.r*c2.i + c1.i*c2.r;
	return c;
}

Complex div(Complex c1, Complex c2) {
	Complex c;
	int a = c1.r;
	int b = c1.i;
	int ac = c2.r;
	int bc = c2.i;
	c.r = (ac*a+b*bc)/(a^2+b^2);
	c.i = (a*bc - ac*b)/(a^2+b^2);
	return c;
}



int main() {
	Complex c1 = getComplex((char *)"Complex 1: ");
	Complex c2 = getComplex((char *)"Complex 2: ");

	show(c1);
	show(c2);

	Complex c = add(c1, c2);
	printf("add result: ");
	show(c);

	c = minus(c1, c2);
	printf("minus result: ");
	show(c);

	c = mul(c1, c2);
	printf("multiply result: ");
	show(c);

	c = div(c1, c2);
	printf("divide result: ");
	show(c);
}