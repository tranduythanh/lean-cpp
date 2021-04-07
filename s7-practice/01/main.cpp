#include "stdio.h"
#include <iostream>
#include <map>

using namespace std;

char encodeHex(int n) {
	return (char)(n+55);
}

void baseN(int x, int n) {
	if (x == 0) return;
	baseN(x/n, n);
	if (x%n > 9) {
		printf("%c", encodeHex(x%n));
		return;
	}
	printf("%d", x%n);
}

int main() {
	int x;
	printf("x = ");
	scanf("%d", &x);

	printf("base  2: ");
	baseN(x, 2);
	cout << endl;
	
	printf("base 16: ");
	baseN(x, 16);
	cout << endl;
}
