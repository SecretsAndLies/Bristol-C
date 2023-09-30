#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

void test(void);
int main(void)
{
    printf("Enter characters (enter 1 to stop");
    char c;
    while (scanf("%c",&c)==1){
        if c=='1'{
            printf("\n");
            return 0;
        }
        if (is_vowel(to_lower(c))){
            c=toupper(c);
        }
        printf("%c",c);
    }
    printf("\n");
    return 0;
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


void test(void)
{
    assert(is_vowel('a')==true);
    assert(is_vowel('z')==false);
}

