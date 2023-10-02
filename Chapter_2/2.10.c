#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>

int secret(int c);
void test(void);
int main(void)
{
    test();
    char c;
    while (scanf("%c",&c)==1){
       printf("%c", secret(c));
    }
    
    return 0;
}

int secret(int c)
{
    if (!isalpha(c)){
        return c;
    }
    
    int ch = 0;

    if (isupper(c)){
        ch = 'A';
    }
    else{
        ch = 'a';
    }

    int c_index = c-ch;

    return (ch+(25-c_index));
}


void test(void)
{
    assert(secret('a')=='z'); // 0 25
    assert(secret('y')=='b'); // 24 1
    assert(secret('T')=='G');
    assert(secret('!')=='!');
}

