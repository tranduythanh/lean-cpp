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

struct team {
	char  name[M];
	int   win;
	int   lost;
	int   draw;
	float score;

	team() {
		score = 0;
	}

	void print() {
		printf("%s\t%d-%d-%d\t%.2f\n", name, win, lost, draw, score);
	}

	void calScore() {
		score = float(win*3 + draw)/(win+lost+draw);
	}
};

struct teamList {
	team arr[1000];
	int size;
	
	teamList() {
		size = 0;
	}

	void add(team s) {
		arr[size] = s;
		arr[size].calScore();
		size++;
	}

	void print() {
		printf("\n");
		for (int i = 0; i < size; i++)
			arr[i].print();
		printf("\n");
	}

	void swap(int i, int k) {
		team t = arr[i];
		arr[i]=arr[k];
		arr[k]=t;
	}

	void sortByScore() {
		for (int i=size-1; i>=0; i--)
			for (int k=i-1; k>=0; k--)
				if (arr[i].score > arr[k].score)
					this->swap(i,k);
	}

	teamList filterByWin(int f) {
		teamList l;
		for (int i = 0; i < size; i++)
			if (arr[i].win == f)
				l.add(arr[i]);
		return l;
	}
};

team inputTeam() {
	team a;
	printf("team Info:\n");
	printf("Name: ");
	scanf("%s", a.name);
	printf("Win:  ");
	scanf("%d", &a.win);
	printf("Lost: ");
	scanf("%d", &a.lost);
	printf("Draw: ");
	scanf("%d", &a.draw);
	return a;
}

int main() {
	teamList ls;
	int n;
	printf("Number of teams: ");
	scanf("%d", &n);

	for (int i=0; i<n; i++) ls.add(inputTeam());

	printf("List of team");
	ls.print();

	ls.sortByScore();
	printf("Sort by score");
	ls.print();

	printf("team 1st\n");
	team firstTeam = ls.arr[0];
	firstTeam.print();
	
	printf("Not yet win");
	teamList newLs = ls.filterByWin(0);
	newLs.print();
	return 0;
}
