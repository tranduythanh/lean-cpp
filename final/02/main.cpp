#include "stdio.h"
#include "math.h"
#include <algorithm>
#include <string>

#define M 100

using namespace std;

bool lessString(char c[M], char d[M]) {
	for (int i = 0; i < M; i++) {
		if (c[i]=='\0' && d[i] != '\0') return true;
		if (c[i]!='\0' && d[i] == '\0') return false;
		if (c[i]=='\0' && d[i] == '\0') return false;
		if (c[i] < d[i]) return true;
		if (c[i] > d[i]) return false;
	}
	return false;
}

struct student {
	char name[M];
	float avgScore;
	int identity;
	
	void print() {
		printf("%s\t%d\t%.2f\n", name, identity, avgScore);
	}
};

struct studentList
{
	student arr[1000];
	int size;

	studentList() {
		size = 0;
	}

	void add(student s) {
		arr[size] = s;
		size++;
	}

	void print() {
		printf("\n");
		for (int i = 0; i < size; i++)
			arr[i].print();
		printf("\n");
	}

	void swap(int i, int k) {
		student t = arr[i];
		arr[i] = arr[k];
		arr[k] = t;
	}

	void sortByName() {
		for (int i = 0; i < size; i++)
			for (int k = i + 1; k < size; k++)
				if (lessString(arr[k].name, arr[i].name))
					this->swap(i, k);
	}
};

student inputStudent(int i) {
	student a;
	printf("Input info of student #%d:\n", i);
	printf("Name:          ");
	scanf("%s", a.name);
	printf("Identity:      ");
	scanf("%d", &a.identity);
	printf("Average Score: ");
	scanf("%f", &a.avgScore);
	return a;
}

int main()
{
	studentList ls;
	int n;
	printf("Number of students: ");
	scanf("%d", &n);

	for (int i = 0; i < n; i++) {
		printf("\n");
		ls.add(inputStudent(i+1));
	}

	printf("\n========== Result ===========\n");
	printf("\nList of students\n(columns: name, identity, average score)");
	ls.print();
	ls.sortByName();
	printf("List of students sorted by name\n(columns: name, identity, average score)");
	ls.print();
	printf("============ End ============\n");

	return 0;
}
