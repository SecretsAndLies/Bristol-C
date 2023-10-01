#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

bool is_vowel(char c);
void test(void);
int main(void)
{
    char f='a';
    char s='a';
    char t='a';
    
    return 0;
}

char get_next_vowel(char c){
    while (c<='z'){
        if (is_vowel(c)){
            return c;
        }
        c=c++;
    }
    return '!';
}

char get_next_const(char c){
    while (c<='z'){
        if (!is_vowel(c)){
            return c;
        }
        c=c++;
    }
    return '!';
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
    assert(get_next_vowel('a')=='e');
    assert(get_next_vowel('u')=='!');
    assert(get_next_const('a')=='b');
    assert(get_next_const('g')=='h');
    assert(get_next_const('z')=='!');

}
