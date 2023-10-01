#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

bool is_vowel(char c);
void test(void);
int main(void)
{
    
    return 0;
}

void print_vowels_or_non_vowels(int type){
    if (type == 1)
    char first_char = 'a';
        for (int i = 0; i<26; i++){
            printf("%c",first_char+i);
            printf("\n");
    }
}
bool is_vowel(char c)
{
    switch (c){
        case 'a':
            return true;
        case 'e':
            return true;
        case 'i':
            return true;
        case 'o':
            return true;
        case 'u':
            return true;
        default:
            return false;
    }
}

void test(void)
{
}
