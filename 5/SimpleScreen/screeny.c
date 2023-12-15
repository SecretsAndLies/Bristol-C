#include <stdio.h>
#include "neillsimplescreen.h"

int main(void)
{
    printf("HI!\n");
    neillclrscrn();
    int row = 20;
    int col = 40;
    printf("\033[%i;%iH",row,col);
    neillfgcol(31); //change to red
    printf("How dare you delete me.\n");
    printf("\033[%i;%iH",row,col);
    row++;
    printf("\033[%i;%iH",row,col);
    printf("How you delete me.\n");
//    for (int i=3; i>0; i--){
//        printf("You have %i seconds to live. \n", i);
//        neillbusywait(1);
//    }
//    for (int i=0; i<100; i++){
//        printf("Goodbye. \n");
//    }
    neillfgcol(37); //change to white
    return 0;

}
