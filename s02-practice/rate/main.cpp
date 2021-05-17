#include "stdio.h"
#include "math.h"
#include "assert.h"

int main() {
	int a;
	printf("position: ");
	scanf("%d", &a);
	
	int rate = a==1? 300 : a==2 ? 200: 100;
	printf("rate is %d", rate);

	return 0;
}
