#include <stdlib.h>
#include <stdio.h>
#define NDEBUG
#include <assert.h>

int hailstone(int n);
void test(void);

int main (void)
{
    test();
    int n;
    if (scanf("%i",&n)!=1){
        return 1;
    }

    while(n>1) {
      n=hailstone(n);
      printf("%i\n",n);
    }       
}

int hailstone(int n){
    if (n==1){
    return 1;
    }
    if (n%2==0){
    return n/2;
    }
    // otherwise, it's an odd number.
    return (3*n)+1;
}

void test(void)
{
    assert(hailstone(77)==232);
    assert(hailstone(232)==116);
    assert(hailstone(58)==29);
}
