#include "stdio.h"
#include "math.h"
#include "assert.h"

int main() {
	for(int num5=0; num5<=100/5; num5++) {
		for (int num10=0; num10<=100/10; num10++) {
			for (int num20=0; num20<=100/20;num20++) {
				if (num5*5+num10*10+num20*20 == 100) {
					printf("%d\t%d\t%d\n", num5, num10, num20);
				}
			}
		}
	}
	printf("Done--------");
}
