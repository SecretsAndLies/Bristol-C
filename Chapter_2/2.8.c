#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

bool is_vowel(char c);
void test(void);
int main(void)
{
    int char_num = 1;
    char first_char = 'a';
    for (int i = 0; i<26; i++){
      printf("%i",char_num);
      char_num++;
      printf("%c", first_char+i);
        for (int j=0; j<26; j++){
          printf("%c", first_char+j);
        }
          for (int k=0; k<26; k++){
            printf("%c", first_char+k);
          }
      printf("\n");
    }
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
}

void test(void)
{
}
