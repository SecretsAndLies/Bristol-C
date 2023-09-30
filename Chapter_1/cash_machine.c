#include <stdio.h>
#include <stdlib.h>
#define NOTE 20

int main(void)
{
    int money;
    int remainder;
    int lower;
    int upper;
    do
    {
        printf("How much money would you like ? ");
        scanf("%d", &money);
        remainder = money % NOTE;
        lower = money - remainder;
        upper = money + (NOTE - remainder);
        if (remainder != 0)
        {
            printf("I can give you %i or %i, try again.\n", lower, upper);
        }
    } while (remainder != 0);
    printf("OK , dispensing ..\n.");
    return 0;
}