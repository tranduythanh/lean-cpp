#include "stdio.h"
#include "math.h"
#include "assert.h"

int main() {
	int y;
	printf("Number of years = ");
	scanf("%d", &y);

	float money = 60000;
	for(int i=0; i<y; i++) {
		money *= (1 + 1.2/100);
	}

	printf("result = %f\n", money);
}
