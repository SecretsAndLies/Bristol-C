#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define DENOMINATORS 100000
#define NUM_TO_SEARCH_FOR 3.14159265361893647039
#define NUMERATORS 100000

void search(void);
void test(void);

int main(void)
{
    search();
    return 0;
}

void search(void)
{
    int best_denominator = 0;
    int best_numerator = 0;
    // setting intial high value for later compares.
    long double guess_difference = INFINITY; 
    long double best_frac = 0;
    for(int i=1; i<DENOMINATORS; i++){
        for (int j=1; j<NUMERATORS; j++){
            long double fract = (long double)  j / (long double) i;
            long double current_guess_difference = fabsl(fract-(long double) NUM_TO_SEARCH_FOR);
            if (current_guess_difference<guess_difference){
                best_denominator = i;
                best_numerator = j;
                best_frac = fract;
                guess_difference=current_guess_difference;
            }
        }
    }
    printf("%i / %i = %.15Lf \n", best_numerator,best_denominator, best_frac);
}

void test(void)
{
}

