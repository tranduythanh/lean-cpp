#include "stdio.h"
#include "math.h"
#include <algorithm>
#include <string>

using namespace std;

struct Poly {
	float args[2000];
	int order;

	void print() {
		if (order == 0) {
			printf("%f", args[0]);
		}
		for (int i = order; i >= 0; i--) {
			if (args[i] == 0) continue;
			if (args[i] != 0 && i == 1)      printf("%.2fx", args[i]);
			else if (args[i] == 1 && i != 0) printf("x^%d", i);
			else if (i != 0)                 printf("%.2fx^%d", args[i], i);
			else                             printf("%.2f", args[i]);
			if (i==1 && args[0]==0) continue;
			if (i>0)                         printf(" + ");
		}
		printf("\n");
	}

	float apply(float x) {
		float ret = 0;
		for (int i = order; i >= 0; i--)
			ret += args[i]*pow(x, i);
		return ret;
	}

	Poly derivative() {
		Poly p;
		p.args[order] = 0;
		for (int i = order; i > 0; i--) {
			p.args[i-1] = args[i]*i;
		}
		p.order = order -1;
		if (p.order < 0) p.order = 0;
		return p;
	}

	Poly derivativeK(int k) {
		Poly p = *this;
		for (int i=0; i<k; i++)
			p = p.derivative();
		return p;
	}

	Poly add(Poly a) {
		Poly x;
		Poly b;
		if (this->order < a.order) {
			b = a;
			x = *this;
		} else {
			b = *this;
			x = a;
		}
		for (int i = 0; i <= x.order; i++)
			b.args[i] = b.args[i]+x.args[i];
		return b;
	}

	Poly mul(Poly a) {
		Poly ret;
		ret.order = this->order+a.order;
		for (int i=0; i<=ret.order; i++) ret.args[i]=0;
		for (int i=0; i<=this->order; i++)
			for (int k=0; k<=a.order; k++)
				ret.args[i+k] += this->args[i]*a.args[k];
		return ret;
	}
};

Poly inputPoly() {
	Poly p;
	printf("Input your polynomial:\n");
	printf("Order: ");
	scanf("%d", &p.order);
	for (int i=0; i<=p.order; i++) {
		printf("argument of order #%d: ", i);
		scanf("%f", &p.args[i]);
	}
	return p;
}

int main() {
	Poly p = inputPoly();
	p.print();
	printf("x=2 => p=%.2f\n", p.apply(2));
	
	Poly p2 = p.derivative();
	printf("Derivative: ");
	p2.print();
	
	int k = 3;
	Poly p3 = p.derivativeK(k);
	printf("Derivative %d: ", k);
	p3.print();

	Poly a = inputPoly();
	a.print();
	
	Poly p4 = p.add(a);
	printf("Add: ");
	p4.print();

	Poly p5 = p.mul(a);
	printf("Mul: ");
	p5.print();

	return 0;
}
