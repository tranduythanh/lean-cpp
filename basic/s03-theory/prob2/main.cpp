#include "stdio.h"

int calculateMonthDays(int m, int y);
int calculateFebDays(int y);
void nexFullDate(int &d, int &m, int &y);

int main() {
	int d,m,y,c;

	printf("Input d,m,y:\n");
	scanf("%d%d%d", &d, &m, &y);
	
	printf("Total days: ");
	scanf("%d", &c);

	printf("%d/%d/%d\n",d,m,y);
	for (int i=1;i<=c;i++) {
		nexFullDate(d, m, y);
		printf("%2d/%2d/%2d\n",d,m,y);
	}
	return 0;
}

void nexFullDate(int &d, int &m, int &y) {
	int days = calculateMonthDays(m, y);
	if (d < days) {
		d++;
		return;
	}

	d = 1;
	if (m <12) {
		m++;
		return;
	}

	m = 1;
	y++;
	return;
}

int calculateMonthDays(int m, int y) {
	switch(m) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return 31;
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
		default:
			return calculateFebDays(y);
	}
}

int calculateFebDays(int y) {
	if (y%4 != 0) return 28;
	if (y/100 % 4 != 0) return 28;
	return 29;
}
