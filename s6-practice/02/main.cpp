#include "stdio.h"
#include "math.h"
#include "assert.h"

int main() {
	float f;
	printf("f = ");
	scanf("%f", &f);

	char r[30]="";

	int i = 0;
	while (f > 0 && i < 20) {
		f *= 2;
		if (f >= 1.0) {
			r[i] = '1';
			f = f-1;
		} else {
			r[i] = '0';
		}
		i++;
	}

	printf("result = 0.%s\n", r);
}
