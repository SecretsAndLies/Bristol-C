/* Try to write power(a,b) to computer a^b
   without using any maths functions other than
   multiplication :
   Try (1) iterative then (2) recursive
   (3) Trick that for n%2==0, x^n = x^(n/2)*x^(n/2)

*/

#include <stdio.h>
#include <assert.h>

int power(unsigned int a, unsigned int b);

int main(void)
{

   int x = 2;
   int y = 16;

   printf("%d^%d = %d\n", x, y, power(x,y));
    assert(power(2,2)==4);
    assert(power(4,7)==16384);
    assert(power(2,16)==65536);
}

int power(unsigned int a, unsigned int b)
{
    if (b==1)
    {
        return a;
    }
    return a * power(a, b-1);
}
