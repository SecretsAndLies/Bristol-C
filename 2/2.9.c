#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

bool is_valid_sequence(char c, char d, char e);
bool const_vow_const_or_vow_const_vow(char c, char d, char e);
bool is_vowel(char c);
bool is_prime(int n);
void test(void);

int main(void)
{
    test();
    int i = 1;
    char first = 'a';
    char second = 'a';
    char third = 'a';
    while (first<='z'){
        while (second<='z'){
            while (third<='z'){
                if (is_valid_sequence(first,second,third)){
                    printf("%i %c%c%c \n",i,first,second,third);
                    i++;
                }
                third=third+1;
            }
            third = 'a';
            second = second+1;
        }
        third='a';
        second='a';
        first = first+1;
    }
    return 0;
}

bool const_vow_const_or_vow_const_vow(char c, char d, char e)
{
    if ( is_vowel(c) && !is_vowel(d) && is_vowel(e) ){
        return true;
    }
    if ( !is_vowel(c) &&is_vowel(d) && !is_vowel(e) ){
        return true;
    }
    return false;
}
    

bool is_valid_sequence(char c, char d, char e)
{
    // consonant-vowel-consonant or vowel-consonant-vowel.
    if (!const_vow_const_or_vow_const_vow(c,d,e)){
        return false;
    }

    // first and last same
    if(c!=e){
        return false;
    }
    // sums to prime
    return is_prime(c+d+e);
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

bool is_prime(int n)
{
    if (n==1){
        return false;    
    }
    if (n==2){
        return true;
    }    

    for (int i=2; i<n; i++){
        if (n%i==0){
            return false;
        }
    }
    return true;
}

void test(void)
{
    assert(is_valid_sequence('b','o','b')==true);
    assert(is_valid_sequence('a','b','a')==false);

}

