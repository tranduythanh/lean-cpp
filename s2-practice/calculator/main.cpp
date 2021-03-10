#include "stdio.h"
#include "math.h"
#include "assert.h"

int main() {
	int a, b, r;
	char op;

	printf("first integer:      ");
	scanf("%d", &a);
	printf("second integer:     ");
	scanf("%d", &b);
	printf("Operator (+-*DMLN): ");
	scanf(" %c", &op);

	switch (op) {
		case '+':
			r = a+b;
			break;
		case '-':
			r = a-b;
			break;
		case '*':
			r=a*b;
			break;
		case 'D':
		case 'd':
			r=a/b;
			break;
		case 'M':
		case 'm':
			r = a%b;
			break;
		case 'L':
		case 'l':
			r = a>b? a:b;
			break;
		case 'N':
		case 'n':
			r = a<b? a:b;
			break;
		default:
			printf("unsupported operator");
			return 0;
	}
	printf("result = %d", r);
	return 0;
}
