#include <stdio.h>
#include <stdlib.h>

int main (void){    
    int median = RAND_MAX/2;
    int minus_cnt = 0;
    int plus_count = 0;

    for (int i=0; i<500; i++){
        int random = rand();
        if (random < median){
            minus_cnt++;
        }
        else {
            plus_count++;
        }
        printf("%i ",plus_count-minus_cnt);
    }    
    
    return 0;
}
