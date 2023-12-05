#include "bsa.h"
int rl(int row);
int main(int argc, char *argv[])
{
    bsa * b = bsa_init();
    argc = 0; // fake
    argv[0]='\0'; // fake

    int len = 0;
    for (int i = 0; i < BSA_ROWS; i++){
        bsa_set(b,len,1);
        len +=rl(i);
    }
    bsa_free(b);
    return 0;
}

// row 0 starts at 0
// row 1 starts at 1
// row 2 starts at 3
// row 3 starts at 7
// row 4 starts at 15
// row 5 starts at 31
// row 6 starts at 63
// row 7 starts at 127
// row 8 starts at 255
// row 9 starts at 511
// row 10 starts at 1023
// row 11 starts at 2047
// row 12 starts at 4095
// row 13 starts at 8191
// row 14 starts at 16383
// row 15 starts at 32767
// row 16 starts at 65535
// row 17 starts at 131071
// row 18 starts at 262143
// row 19 starts at 524287
// row 20 starts at 1048575
// row 21 starts at 2097151
// row 22 starts at 4194303
// row 23 starts at 8388607
// row 24 starts at 16777215
// row 25 starts at 33554431
// row 26 starts at 67108863
// row 27 starts at 134217727
// row 28 starts at 268435455
// row 29 starts at 536870911

int rl(int row) 
{
    // same as 2 ^ row
    return 1 << row;
}