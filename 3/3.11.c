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
int get_magnitude(int i);
void test(void);
int add_final_zeroes(int places, char * str, int pos);

int main(void)
{
  test();
  return 0;
}

void test(void)
{
   char s[BIGSTR];
   
   assert(get_first_digit(3141)==3);
   assert(get_first_digit(141)==1);
   assert(get_first_digit(41)==4);
   assert(get_first_digit(10)==1);
   assert(get_first_digit(1)==1);
   assert(get_first_digit(0)==0);
   
   int2string(1, s);
   assert(strcmp(s, "1")==0);
   int2string(0, s);
   assert(strcmp(s, "0")==0);
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
}

void int2string(int i, char* str)
{
    int pos = 0;
    if (is_negative(i)){
        str[pos] = '-';
        pos++;
    }
    if (i==0){
        str[pos] = '0';
        pos++;
    }
    i=abs(i);
    while(i>0){
       int first = get_first_digit(i);
       str[pos] = (char) (first + '0');
       pos++;
       int places = get_magnitude(i);
       // leaves you with an edge case:
       // i==1000 would equal 0. 
       if (places==i && places>=10){
          pos = add_final_zeroes(places, str, pos);
       }
       // eg: if i==3461, you minus 3 * 1000
       i = i - (first * places);
    }
    str[pos] = '\0';
}

int add_final_zeroes(int places, char * str, int pos)
{
    while (places>1){
        str[pos] = '0';
        pos++;
        places = places/10;
    }

    return pos;
}

int get_magnitude(int i)
{
    if (i<10){
        return 1;
    }
    if (i<100){
        return 10;
    }
    if (i<1000){
        return 100;
    }
    if (i<10000){
        return 1000;
    }
   
    // otherwise, give an error.
    return -1;
}

bool is_negative(int i)
{
    return i<0;
}

int get_first_digit(int i)
{
    while (i>=10){
        i = i / 10;
    }
    
    return i;
}

