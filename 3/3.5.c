#include <stdio.h>

#define LINE_LEN

void test(void);

int main(void)
{
    test();
    int top[LINE_LEN] = {0};

    return 0;
}

void fill_with_random_values(arr[LINE_LEN])
{
    for (int i=0; i<LINE_LEN; i++){
        arr[i] = rand()%2;
    }
    print_arr(arr);
}

void print_arr(arr[LINE_LEN])
{
   for (int i=0; i<LINE_LEN; i++){
        printf("%i ", arr[i])
   }
   printf("\n");
}

void test(void)
{
}
