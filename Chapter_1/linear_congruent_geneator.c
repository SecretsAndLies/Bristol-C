
#include <stdio.h>
#include <stdlib.h>
#define A 9
#define C 5
#define M 11
#define LOOPS 10

int main(void)
{
    int seed = 0;
    /* Linear Congruential Generator */
    int first_num;

    for (int i = 0; i < LOOPS; i++)
    {
        seed = (A * seed + C) % M;
        // printf("the generated number is %i \n",seed);
        if (i == 0)
        {
            first_num = seed;
        }
        if (i != 0 && seed == first_num)
        {
            printf("the pattern repeats at index: %i \n", i);
        }
    }
}

// Adapt the above program so that it prints the period of the LCG, where
// you’ve #defined the constants A,C and M and seed always begins at zero. For the constants
// described above, the program would output 5. For A = 7, C = 5 and M = 11 it will output 10.