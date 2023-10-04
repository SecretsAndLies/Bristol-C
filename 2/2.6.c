#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define GOLDEN_RATIO 1.61803398875

bool fibword_phi(int n);

void test(void);
int main(void)
{
   if (fibword_phi(12)){
       printf("Digit is 1 \n");
    }
   else{
       printf("digit is 0 \n");
    }
    return 0;
}

bool fibword_phi(int n){
    double n_x_golden_ratio = (double) n * (double) GOLDEN_RATIO;
    double n_add_1_times_golden_ratio = ((double) n + (double) 1) * (double) GOLDEN_RATIO;
    double answer = (double) 2 + floor(n_x_golden_ratio) - floor(n_add_1_times_golden_ratio);
    return (int) answer;
}

void test(void)
{
}

