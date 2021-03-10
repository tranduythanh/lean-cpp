#include "stdio.h"
#include "math.h"
#include "assert.h"

int donVi(int a) {
	return a%10;
}

int tram(int a) {
	return (a/100)%10;
}

int sumDigit(int a) {
	int sum = 0;
	for (;a>0;) {
		sum += a%10;
		a = a/10;
	}
	return sum;
}

void test() {
	assert(donVi(13)==3);
	assert(donVi(999)==9);
	assert(donVi(7)==7);
	assert(tram(999)==9);
	assert(tram(16)==0);
	assert(tram(8759)==7);
	assert(sumDigit(123)==6);
	assert(sumDigit(7777)==28);
}

int main() {
	test();
	int a;
	printf("positive integer: ");
	scanf("%d", &a);

	if (a < 0) {
		printf("not a positive integer\n");
		return 0;
	}
	
	printf("donvi: %d\n", donVi(a));
	printf("tram:  %d\n", tram(a));
	printf("sum:   %d\n", sumDigit(a));
	return 0;
}
