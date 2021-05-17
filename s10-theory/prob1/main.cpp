#include "stdio.h"
#include <string>

struct ratio {
    int up;
    int down;
};

ratio getRatio(char* msg) {
    ratio r;
    printf("%s", msg);
    scanf("%d/%d", &r.up, &r.down);
    return r;
}

void show(ratio r) {
    printf("ratio: %d/%d\n", r.up, r.down);
}

ratio add(ratio r1, ratio r2) {
    ratio r;
    r.up = r1.up*r2.down + r1.down*r2.up;
    r.down = r1.down * r2.down;
    return r;
}

int gcd(int a, int b)
{
    if (a == 0 || b == 0){
        return a + b;
    }
    while (a != b){
        if (a > b){
            a -= b;
        }
        else{
            b -= a;
        }
    }
    return a;
}

ratio simplify(ratio r) {
    int n = gcd(r.up, r.down);
    r.up /= n;
    r.down /= n;
    return r;
}

int main() {
    ratio r1 = getRatio((char*)"ratio 1: ");
    ratio r2 = getRatio((char*)"ratio 2: ");
    
    show(r1);
    show(r2);

    ratio r = add(r1, r2);
    r = simplify(r);
    printf("result: ");
    show(r);
}