#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define WORD_LEN 100

void test(void);
int roman_str_to_int(char roman[WORD_LEN]);
void fill_int_array(char roman[WORD_LEN], int int_arr[WORD_LEN], int len);
int get_biggest_arr_index(int arr[WORD_LEN], int b, int e);
int roman_char_to_int(char r);

int main(void)
{
    test();
    return 0;
}

int roman_str_to_int(char roman[WORD_LEN])
{
    int int_arr[WORD_LEN];
    int len = strlen(roman);
    fill_int_array(roman, int_arr, len);

    int sum = 0;

    for (int i=0; i<len; i++){
        
        if (get_biggest_arr_index(int_arr, i, len)==i){
            sum+=int_arr[i];
        }
        else {
            sum-=int_arr[i];
        }
    }

    return sum;    
}

/* returns index of largest element in the array
 * within the given indexes */
int get_biggest_arr_index(int arr[WORD_LEN], int b, int e)
{
    int largest = b;
    for (int i=b; i<e; i++){
        if (arr[i]>arr[largest]){
            largest = i;
        }
    }
    return largest;
    
}

/* Creates a copy of the array, but filled with the 
 * coressponding values, eg: VII becomes 5, 1, 1 */
void fill_int_array(char roman[WORD_LEN], int int_arr[WORD_LEN], int len)
{
    for (int i=0; i<len; i++){
        int_arr[i] = roman_char_to_int(roman[i]); 
    }
}

int roman_char_to_int(char r)
{
    switch (r) {
        case 'I':
              return 1;
              break;
        case 'V':
              return 5;
              break;
        case 'X':
              return 10;
              break;
        case 'L':
              return 50;
              break;
        case 'C':
              return 100;
              break;
        case 'D':
              return 500;
              break;
        case 'M':
              return 1000;
              break;
    }

    // quit on invalid input.
    printf("Invalid Character Entered. \n");
    exit(1);
}

void test(void)
{
    
    assert(roman_char_to_int('I')==1);
    assert(roman_char_to_int('V')==5);
    assert(roman_char_to_int('X')==10);
    assert(roman_char_to_int('L')==50);
    assert(roman_char_to_int('C')==100);
    assert(roman_char_to_int('D')==500);
    assert(roman_char_to_int('M')==1000);

    int a[] = {1,2,3,4,5,6,7,8,9,10};
    assert(get_biggest_arr_index(a, 0, 10)==9);
    int b[] = {1,100,3,4,5,6,7,8,9,10};
    assert(get_biggest_arr_index(b, 0, 10)==1);

    assert(roman_str_to_int("I")==1);
    assert(roman_str_to_int("V")==5);
    assert(roman_str_to_int("X")==10);
    assert(roman_str_to_int("L")==50);
    assert(roman_str_to_int("C")==100);
    assert(roman_str_to_int("D")==500);
    assert(roman_str_to_int("M")==1000);

    assert(roman_str_to_int("VI")==6);
    assert(roman_str_to_int("VIII")==8);
    assert(roman_str_to_int("IV")==4);
    assert(roman_str_to_int("IIV")==3);


    assert(roman_str_to_int("MCMLXVI")==1966);
    assert(roman_str_to_int("MCMLXVII")==1967);
    assert(roman_str_to_int("MCMXCIX")==1999);
    assert(roman_str_to_int("MCDXCI")==1491);
}
