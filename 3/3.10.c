#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX 280

bool is_palindrome(char a[MAX]);
int len_of_string(char a[MAX]);

void test(void);

int main(void)
{
    test();
    return 0;
}

bool is_palindrome(char a[MAX])
{
    int i=0;
    int j=len_of_string(a)-1;
    
    while (i<=j)
    {
        char c = tolower(a[i]);
        char d = tolower(a[j]);
       
        if (!isalpha(c)){
            i++;
            continue;
        }
        
        if (!isalpha(d)){
            j--;
            continue;
        }

        if(c!=d){
            return false;
        }
        i++;
        j--;
    }
    return true;
}

int len_of_string(char a[MAX])
{
    int len = 0;
    char c = a[len];
    while (c!='\0'){
        len++;
        c=a[len];
    }
    return len;
}

void test(void)
{
    assert(len_of_string("test")==4);
    assert(is_palindrome("kayak")==true);
    assert(is_palindrome("A man, a plan, a canal: Panama!")==true);
    assert(is_palindrome("Madam, in Eden I’m Adam,")==true);
    assert(is_palindrome("Level, madam, level!")==true);
}
