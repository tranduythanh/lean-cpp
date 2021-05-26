#include "stdio.h"
#include "math.h"
#include <algorithm>
#include <string>

#define M 20

using namespace std;

bool compare(char c[M], char d[M]) {
	for (int i = 0; i < M; i++) {
		if (c[i] == '\0') break;
		if (c[i] != d[i]) return false;
	}
	return true;
}

struct student {
	char  identity[M];
	int   family;
	float score;
	int   age;

	void print() {
		printf("%s\t%d\t%.2f\t%d\n", identity, family, score, age);
	}
};

struct studentList {
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
		arr[i]=arr[k];
		arr[k]=t;
	}

	void sortByScore() {
		for (int i=0;i<size; i++)
			for (int k=i+1; k<size; k++)
				if (arr[i].score > arr[k].score)
					this->swap(i,k);
	}

	studentList filterByFamily(int f) {
		studentList l;
		for (int i = 0; i < size; i++)
			if (arr[i].family == f)
				l.add(arr[i]);
		return l;
	}

	student searchByIdentity(char c[M]) {
		student a;
		for (int i = 0; i < size; i++) 
			if (compare(arr[i].identity, c)) 
				return arr[i];
		return a;
	}
};

student inputStudent() {
	student a;
	printf("Student Info:\n");
	printf("Identity: ");
	scanf("%s", a.identity);
	printf("Family:   ");
	scanf("%d", &a.family);
	printf("Score:    ");
	scanf("%f", &a.score);
	printf("Age:      ");
	scanf("%d", &a.age);
	return a;
}

int main() {
	studentList ls;
	int n;
	printf("Number of students: ");
	scanf("%d", &n);

	for (int i=0; i<n; i++) ls.add(inputStudent());

	printf("List of student");
	ls.print();

	ls.sortByScore();
	printf("Sort by score");
	ls.print();
	
	char id[M];
	printf("Search by identity: ");
	scanf("%s", id);
	student a = ls.searchByIdentity(id);
	a.print();

	printf("Not have family");
	studentList newLs = ls.filterByFamily(1);
	newLs.print();
	return 0;
}
