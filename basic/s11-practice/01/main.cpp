#include "stdio.h"
#include "math.h"
#include <string>

struct ratio {
	int up;
	int down;
};

int gcd(int a, int b) {
	if (a == 0 || b == 0) {
		return a + b;
	}
	while (a != b) {
		if (a > b)
		{
			a -= b;
		}
		else
		{
			b -= a;
		}
	}
	return a;
}

ratio simplify(ratio r) {
	int sign = 1;
	if (r.up*r.down < 0) {
		sign = -1;
	}
	r.up = abs(r.up);
	r.down = abs(r.down);

	int n = gcd(r.up, r.down);
	r.up /= n;
	r.down /= n;
	r.up *= sign;
	return r;
}

ratio getRatio(char *msg) {
	ratio r;
	printf("%s", msg);
	scanf("%d/%d", &r.up, &r.down);
	return r;
}

void show(ratio r) {
	printf("ratio: %d/%d\n", r.up, r.down);
}

ratio add(ratio r1, ratio r2) {
	ratio r;
	r.up = r1.up * r2.down + r1.down * r2.up;
	r.down = r1.down * r2.down;
	return simplify(r);
}

ratio minus(ratio r1, ratio r2) {
	ratio r;
	r.up = r1.up * r2.down - r1.down * r2.up;
	r.down = r1.down * r2.down;
	return simplify(r);
}

ratio mul(ratio r1, ratio r2) {
	ratio r;
	r.up = r1.up * r2.up;
	r.down = r1.down * r2.down;
	return simplify(r);
}

ratio div(ratio r1, ratio r2) {
	ratio r;
	r.up = r1.up * r2.down;
	r.down = r1.down * r2.up;
	return simplify(r);
}



int main() {
	ratio r1 = getRatio((char *)"ratio 1: ");
	ratio r2 = getRatio((char *)"ratio 2: ");

	show(r1);
	show(r2);

	ratio r = add(r1, r2);
	printf("add result: ");
	show(r);

	r = minus(r1, r2);
	printf("minus result: ");
	show(r);

	r = mul(r1, r2);
	printf("multiply result: ");
	show(r);

	r = div(r1, r2);
	printf("divide result: ");
	show(r);
}