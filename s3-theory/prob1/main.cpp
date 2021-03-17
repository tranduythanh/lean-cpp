#include "stdio.h"

void printMonthDays(int y);
int  calculateFebDays(int y);
void printDays(int month, int days, int y);

int main() {
	int y1;

	printf("Input year: ");
	scanf("%d", &y1);
	
	printMonthDays(y1);

	int y2;
	printf("Input year: ");
	scanf("%d", &y2);
	
	printMonthDays(y2);

	return 0;
}

void printMonthDays(int y) {
	for (int i = 1; i <= 12; i++) {
		int days = 31;
		switch(i) {
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
				break;
			case 4:
			case 6:
			case 9:
			case 11:
				days = 30;
				break;
			default:
				days = calculateFebDays(y);
				break;
		}
		printDays(i, days, y);
	}
}

void printDays(int month, int days, int y) {
	for (int d = 1; d<=days;d++) {
		printf("%2d/%2d/%d\n", d, month, y);
	}
}

int calculateFebDays(int y) {
	if (y%4 != 0) return 28;
	if (y/100 % 4 != 0) return 28;
	return 29;
}
