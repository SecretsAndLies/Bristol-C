// How many numbers do you wish to enter ? 5
// Enter 5 real numbers: 1.01 −3 2.2 7.0700 5
// Maximum value: 7.07

#include <stdio.h>
#include <stdlib.h>

int main (void){
    printf("How many numbers do you want to enter?  ");
    int num_of_nums;
    scanf("%i", &num_of_nums);
    int max = 0;
    int n=0;
    printf("Enter 5 real numbers   ");
    for (int i=0; i<num_of_nums; i++){
        scanf("%i", &n);
        if(n>max){
            max=n;
        }
    }
    printf("maxmium value: %i",max);
    return 0;
}