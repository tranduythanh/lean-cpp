#include "stdio.h"
#include <iostream>
#include <math.h>

using namespace std;

int add(int s, int n) {
	if (n==0) return s;
	return add(s+n%10, n/10);
}

int innerSum(int n) {
	int s = 0;
	return add(s, n); 
}

int main() {
	int n;
	printf("n = ");
	scanf("%d", &n);
	printf("inner sum = %d\n", innerSum(n));
}
