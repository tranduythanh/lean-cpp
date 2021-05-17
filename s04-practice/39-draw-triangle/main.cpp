#include "stdio.h"
#include "math.h"
#include "assert.h"

void drawDownTriangle(int size);
void drawUpTriangle(int size);
void drawMidTriangle(int size);
void printChar(char c, int n);

int main() {
	int size;
	printf("triangle size: ");
	scanf("%d", &size);
	drawDownTriangle(size);
	printf("\n");
	drawUpTriangle(size);
	printf("\n");
	drawMidTriangle(size);
	printf("\n");
}

void printChar(char c, int n) {
	for (int i=0; i<n;i++) {
		printf("%c", c);
	}
}

void drawDownTriangle(int size) {
	for (int i = 1; i <= size; i++) {
		printChar('A', size-i+1);
		printf("%c", '\n');
	}
}

void drawUpTriangle(int size) {
	for (int i=1; i<=size; i++) {
		printChar('A', i);
		printf("%c", '\n');
	}
}

void drawMidTriangle(int size) {
	for (int i=1; i<= size; i++) {
		printChar(' ', size-i);
		printChar('A', 2*i-1),
		printf("%c", '\n');
	}
}