#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

char get_next_vowel(char c);
char get_next_const(char c);
bool is_vowel(char c);
void test(void);
int main(void)
{
    test();
    int i = 1;
    char first = get_next_const('a');
    char second = get_next_vowel('a');
    char third = get_next_const('a');
    while (first!='!'){
        while (second!='!'){
            while (third!='!'){
                printf("%i %c%c%c \n",i,first,second,third);
                third=get_next_const(third+1);
                i++;
            }
            third = get_next_const('a');
            second = get_next_vowel(second+1);
        }
        third=get_next_const('a');
        second=get_next_vowel('a');
        first = get_next_const(first+1);
    }
    return 0;
}

char get_next_vowel(char c){
    while (c<='z'){
        if (is_vowel(c)){
            return c;
        }
        c++;
    }
    return '!';
}

char get_next_const(char c){
    while (c<='z'){
        if (!is_vowel(c)){
            return c;
        }
        c++;
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
    assert(get_next_vowel('a')=='a');
    assert(get_next_vowel('b')=='e');
    assert(get_next_vowel('v')=='!');
    assert(get_next_const('a')=='b');
    assert(get_next_const('g')=='g');
    assert(get_next_const('z')=='z');

}
