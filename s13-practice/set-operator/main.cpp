#include "stdio.h"
#include "math.h"
#include <algorithm>
#include <string>

using namespace std;

struct Set {
	int arr[2000];
	int size;

	void print() {
		printf("{");
		for (int i = 0; i < this->size; i++) {
			printf("%d", arr[i]);
			if (i != this->size -1)
				printf(",\t");
		}
		printf("}\n");
	}

	bool contains(int elem) {
		for (int i=0; i< this->size; i++)
			if (elem == this->arr[i])
				return true;
		return false;
	}

	Set intersec(Set b) {
		Set c;
		c.size = 0;
		for (int i = 0; i < this->size; i++)
			if (b.contains(this->arr[i])) {
				c.arr[c.size] = this->arr[i];
				c.size++;
			}
		return c;
	}

	Set minus(Set b) {
		Set c;
		c.size = 0;
		for (int i = 0; i < this->size; i++)
			if (!b.contains(this->arr[i])) {
				c.arr[c.size] = this->arr[i];
				c.size++;
			}
		return c;
	}

	bool in(Set b) {
		Set c = this->minus(b);
		return c.size == 0;
	}
};

Set inputSet() {
	Set a;
	printf("Input your set:\n");
	printf("Size: ");
	scanf("%d", &a.size);
	for (int i=0; i<a.size; i++) {
		printf("element #%d: ", i);
		scanf("%d", &a.arr[i]);
	}
	return a;
}

int main() {
	Set a = inputSet();
	a.print();

	Set b = inputSet();
	b.print();

	Set c = a.intersec(b);
	c.print();

	Set d = a.minus(b);
	d.print();

	bool ok = a.in(b);
	printf("%d\n", ok);

	return 0;
}
