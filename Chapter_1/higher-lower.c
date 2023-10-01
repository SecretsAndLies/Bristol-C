
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int random = rand()%1000;
    int goes_remaining=10;
    int guess;
    printf("I'm thinking of a number, try to guess what it is. ");
    while (goes_remaining > 0)
    {
        printf("You have %i goes remaining. \n", goes_remaining);
        scanf(" %d", &guess);
        if (guess == random)
        {
            printf("You guessed it! \n");
            return 0;
        }
        if (guess > random)
        {
            printf("too high! \n");
        }
        if (guess < random)
        {
            printf("too low! \n");
        }
        
    goes_remaining--;
    }
    printf("hahaa, I win. You ran out of goes.\n");
    return 0;
}
