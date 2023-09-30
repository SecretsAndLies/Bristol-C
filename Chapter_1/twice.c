#include <stdio.h>
int main(void)
{
    int a, b, c;
    printf("Input 3 Intergers: ");
    scanf("%d", &a);
    scanf("%d", &b);
    scanf("%d", &c);
    printf("Twice the sum of the intergers + 7 is %i \n", ((a + b + c) * 2) + 7);

    int i;
    i = 0;

    while (i < 10) {
        printf("%i", i);
    }
}