#include <stdio.h>
#include <stdlib.h>

void print_arr(int numbers[], int length);
void test(void);
void shuffle_array(int random_numbers[], int length);

int main(void)
{
    test();
    printf("how many songs? ");
    int amount;
    if(scanf("%d", &amount) != 1)
    {
        return 1;
    }

    int numbers[100];

    for (int i=0; i<amount; i++){
        numbers[i]=i+1;
    }
    
    //print_arr(numbers, amount);

    shuffle_array(numbers, amount);
    
    print_arr(numbers, amount);

    return 0;
}

void print_arr(int numbers[], int length)
{
    for (int i=0; i<length; i++){
        printf("%i ", numbers[i]);
    }

    printf("\n");
}

void shuffle_array(int numbers[], int length){  
    
    for (int i=0; i<length; i++) {
        int random1 = rand() % length;
        int random2 = rand() % length;
    
        int temp = numbers[random1];
        numbers[random1] = numbers[random2];
        numbers[random2] = temp;
    }
}




void test(void)
{
}
