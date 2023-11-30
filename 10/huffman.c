#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>

#define LEN 1000
#define ASCI 256

int main(int argc, char *argv[]){
    if(argc!=2){
        return 1;
    }
    char ch;
    FILE *f;
    f = fopen(argv[1], "r");

    int table[ASCI];
    do {
        ch = fgetc(f);
        printf("%c", ch);
    } while (ch != EOF);

    fclose(f);
}



//Each character is shown, along with its Huffman bit-pattern, 
// the length of the bit-pattern and
// the frequency of occurrence. 
// At the bottom, the total number of bytes required to compress the
// file is displayed.