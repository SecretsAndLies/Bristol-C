#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define LOOP 1000000000
#define PI 3.14159265
#define TOLERANCE 0.00000001

int lebiniz(void);
void test(void);

int main(void)
{
    int num_loops_requied = lebiniz();
    printf("%i \n",num_loops_requied);
    return 0;
}

int lebiniz(void){
    int numerator = 4;
    double pi = 4;
    bool last_sign_was_plus = true;
    double fraction;
    for (int i=3; i<LOOP; i+=2){
        fraction = (double) numerator / (double) i;
        if(last_sign_was_plus){
            pi-=fraction;
            last_sign_was_plus = false;
        }
        else {
            last_sign_was_plus = true;
            pi+=fraction;
        }
        if (fabs(pi-PI)<TOLERANCE){
            return i;
        }
    }
    return 0;
}

void test(void)
{
    return;
}
