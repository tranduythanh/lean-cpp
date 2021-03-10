#include "stdio.h"
#include "math.h"
#include "assert.h"

#define PI atan(1)*4

float circumferenceSquare(float a) {
	return a*4;
}

float areaRhombus(float a, float b) {
	return a*b/2;
}

float areaCircle(float r) {
	return PI*pow(r,2);
}

float areaTriangle(float a, float b, float c) {
	float h = (a+b+c)/2;
	return sqrt(h*(h-a)*(h-b)*(h-c));
}

bool isTriangle(float a, float b, float c) {
	return a+b>c && b+c>a && c+a>b;
}

void test() {
	assert(isTriangle(1,2,3)==false);
	assert(isTriangle(3,4,5)==true);
	assert(isTriangle(0,1,1)==false);
}

int main() {
	test();
	int choice;
	printf("1 calculate circumference of Square\n");
	printf("2 calculate area of Rhombus\n");
	printf("3 calculate area of Circle\n");
	printf("4 calculate area of Triangle\n");
	printf("Your choice (type the number): \n");
	scanf("%d", &choice);
	
	switch(choice) {
		case 1: 
			{
				float edgeSize;
				printf("edge size: ");
				scanf("%f", &edgeSize);
				float p = circumferenceSquare(edgeSize);
				printf("circumference of square: %f", p);
			}
			break;
		case 2: 
			{
				float dia1, dia2;
				printf("diagonals:\n");
				scanf("%f%f", &dia1, &dia2);
				float s1 = areaRhombus(dia1, dia2);
				printf("area of Rhombus: %f", s1);
			}
	  		break;
		case 3:
			{
				float r;
				printf("radius: ");
				scanf("%f", &r);
				float s2 = areaCircle(r);
				printf("area of Circle: %f", s2);
			}
			break;
		case 4:
			{
				float a, b, c;
				printf("3 edges:\n");
				scanf("%f%f%f", &a, &b, &c);
				if (! isTriangle(a,b,c)) {
					printf("invalid edges\n");
					return 0;
				}
				float s3 = areaTriangle(a,b,c);
				printf("area of Triangle: %f", s3);
			}
			break;
		default:
			printf("unsupported action");	
	};
	
	return 0;
}
