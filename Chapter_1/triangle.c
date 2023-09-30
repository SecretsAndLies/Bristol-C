#include <stdio.h>
#include <stdlib.h>
#define LOOP_COUNT 100

int main(void)
{
    for (int num = 0; num < LOOP_COUNT; num++)
    {
        int sum = 0;
        for (int i = num; i > 0; i--)
        {
            sum += i;
        }
        printf("%i \n", sum);
    }
}
