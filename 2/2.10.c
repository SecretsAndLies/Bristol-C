#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>

#define MAX_LETTER_INDEX 25

int secret(int c);
void test(void);
int main(void)
{
    test();
    char c = 'a';
    printf("Enter strings and I will encode them (control D to quit) \n");
    while (c != EOF) {
        c = getchar();
        putchar(secret(c));
    }
    
    return 0;
}

int secret(int c)
{
    // if value isn't a alphabetical character, we return unchanged.
    if (!isalpha(c)){
        return c;
    }
    
    int case_index = 0;
    
    if (isupper(c)){
        case_index = 'A';
    }
    else{
        case_index = 'a';
    }

    // creates a zero indexed version of the char
    // eg: a=0, B=1
    int original_c_index = c-case_index; 
   
    // as above, but encodes it, eg: 0 becomes 25
    // (a becomes z)
    int new_c_index = MAX_LETTER_INDEX-original_c_index;

    // then we add the case back in,
    // eg: 25 becomes 90 (the code for uppercase Z)
    return (case_index+new_c_index);
}


void test(void)
{
    assert(secret('a') == 'z');
    assert(secret('y') == 'b');
    assert(secret('T') == 'G');
    assert(secret('!') == '!');
}

