#include "stdio.h"
#include "math.h"
#include <string>

using namespace std;

struct Student {
	char name[100];
	float  math;
	float  inf;
};

Student getStudent(char *msg) {
	Student c;
	printf("-----------------\n");
	printf("%s", msg);
	printf("Name: ");
	scanf("%s", c.name);
	printf("math: ");
	scanf("%f", &c.math);
	printf("inf:  ");
	scanf("%f", &c.inf);
	return c;
}

Student highestMath(Student* ss, int n) {
	Student s = {"",0,0};
	for (int i = 0; i < n; i++) {
		if (ss[i].math <= s.math) {
			continue;
		}
		s = ss[i];
	}
	return s;
}

Student highestInf(Student *ss, int n)
{
	Student s = {"", 0, 0};
	for (int i = 0; i < n; i++)
	{
		if (ss[i].inf <= s.inf)
		{
			continue;
		}
		s = ss[i];
	}
	return s;
}

float avgMath(Student *ss, int n) {
	float avg = 0;
	for (int i = 0; i < n; i++) {
		avg += ss[i].math;
	}
	return avg/n;
}

float avgInf(Student *ss, int n) {
	float avg = 0;
	for (int i = 0; i < n; i++) {
		avg += ss[i].inf;
	}
	return avg/n;
}

void show(Student c) {
	printf("---------------\nName: %s\nMath: %.2f\nInf:  %.2f\n", c.name, c.math, c.inf);
}

int main() {
	int n;
	Student ss[1000];
	char buffer[100];
	printf("Number of student: ");
	scanf("%d", &n);
	for (int i=0; i<n; i++) {
		sprintf(buffer, "Student %4d\n", i);
		Student s = getStudent(buffer);
		ss[i] = s;
	}

	Student s1 = highestMath(ss, n);
	Student s2 = highestInf(ss, n);
	float avgScoreMath = avgMath(ss, n);
	float avgScoreInf = avgInf(ss, n);

	printf("Student highest math score: %s - %.2f\n", s1.name, s1.math);
	printf("Student highest inf score:  %s - %.2f\n", s2.name, s2.inf);
	printf("Average math score:         %.2f\n", avgScoreMath);
	printf("Average inf score:          %.2f\n", avgScoreInf);

	return 0;
}