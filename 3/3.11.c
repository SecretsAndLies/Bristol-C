#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

#define BIGSTR 1000

bool is_negative(int i);
int get_first_digit(int i);
void int2string(int i, char* str);

int main(void)
{
   char s[BIGSTR];

   assert(get_first_digit(3141)==3);
   
   int2string(0, s);
//   assert(strcmp(s, "0")==0);
   int2string(10, s);
   assert(strcmp(s, "10")==0);
   int2string(99, s);
   assert(strcmp(s, "99")==0);
   int2string(124, s);
   assert(strcmp(s, "124")==0);
   int2string(3141, s);
   assert(strcmp(s, "3141")==0);
   int2string(-1000, s);
   assert(strcmp(s, "-1000")==0);
   return 0;
}

void int2string(int i, char* str)
{
    int pos = 0;
    
    if (is_negative(i)){
        str[pos] = '-';
        pos++;
    }
    
    i=abs(i);

    while(i>0){
       int first = get_first_digit(i);
       str[pos] = (char) (first + '0');
       // minus the resulting amount of places (eg: if 3141 
       // you need to subtract the first digit 3 * 1000
   }

    str[pos] = '\0';
}

int get_magnitude(int i)
{

}

bool is_negative(int i)
{
    return i<0;
}

int get_first_digit(int i)
{
    while (i>10){
        i = i / 10;
    }
    
    return i;
}

