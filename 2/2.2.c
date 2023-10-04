#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

void test(void);
int unit_circle(int x);

int main(void)
{
    test();
    int x=0;
    printf("enter your value \n");
    scanf("%i",&x);
    printf("unit circle is %i \n",unit_circle(x));
    return 0; 
}

int unit_circle(int x)
{
   return pow(sin(x),2)+pow(cos(x),2);
}

void test(void)
{
    assert(unit_circle(12)==1);
}
