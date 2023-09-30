
#include <stdio.h>
#include <stdlib.h>

int isEven(int n);

int main(void)
{
    printf("How many numbers do you want to enter?  ");
    int num_of_nums;
    scanf("%i", &num_of_nums);
    int max = 0;
    int n = 0;
    printf("Enter 5 real numbers (even numbers are ignored)  ");
    for (int i = 0; i < num_of_nums; i++)
    {
        scanf("%i", &n);
        if (isEven(n)==0 && n > max)
        {
            max = n;
        }
    }
    printf("maxmium value: %i \n", max);
    return 0;
}

int isEven(int n)
{
    if (n % 2 == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
