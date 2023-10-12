#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SOUNDEX_LEN 4
#define WORDLEN 100

void test(void);
void soundex(char input[WORDLEN], char output[WORDLEN]);
void pad_arr_with_zeroes(char arr[], int i);
int encode(char a, int i);
bool are_chars_same_code(char a, char b, int index_of_a);
int fill_arr(char input[], char output[]);
void truncate_str(char arr[]);

int main(void)
{
    test();
    return 0;
}

// J250

void soundex(char input[WORDLEN], char output[WORDLEN])
{
  
    int i = fill_arr(input, output);
    pad_arr_with_zeroes(output, i);

    truncate_str(output);
    printf("%s \n",output);
    return;
}

void truncate_str(char arr[])
{
    arr[MAX_SOUNDEX_LEN] = '\0';
}

int fill_arr(char input[], char output[])
{
    int i = 0;
    int j = 0;

    while (input[i]!='\0'){
            char c = input[i];
            char p = input[j-1];
            if (!are_chars_same_code(c,p,i)){
            int encoded = encode(c, i);
                if(encoded!=0){
                    output[j]=(char) encoded+'0';
                    j++;
                }
            }
        i++; 
    }
    // return the index of 
    // the last char of the string
    return i-1;
}

void pad_arr_with_zeroes(char arr[], int i)
{
    while (i<MAX_SOUNDEX_LEN){
        arr[i] = '0';
        i++; 
    }
}

bool are_chars_same_code(char a, char b, int index_of_a)
{
    // b is already encoded.
    return encode(a,index_of_a)==b;
}

int encode(char a, int i)
{
    if (i==0){
        return a;
    }
    int lower = tolower(a);
    int zero_index = lower - 'a'; 
    // this array contains the correct code for each letter
    // at the index, eg con_arr[0] (a) == 0.
    int con_arr[] = {
    0, 1, 2, 3, 0, 1,
    2, 0, 0, 2, 2, 4,
    5, 5, 0, 1, 2, 6,
    2, 3, 0, 1, 0, 2,
    0, 2
    };
    return con_arr[zero_index];
}


// pad with zeroes if not long enough
//


void test(void)
{
    assert(encode('a')==0);
    assert(encode('A')==0);
    assert(encode('b')==1);
    assert(encode('m')==5);
    assert(encode('r')==6);
    assert(encode('D')==3);
    assert(encode('Z')==2);
    assert(encode('w')==0);
    assert(encode('s')==2);
    assert(encode('h')==0);
    assert(encode('i')==0);
    assert(encode('n')==5);
    assert(encode('g')==2);
    assert(encode('t')==3);
    assert(encode('o')==0);

    char output[WORDLEN];
    
    char test[WORDLEN] = "W";
    pad_arr_with_zeroes(test,1);
    assert(strcmp(test,"W000")==0);

    soundex("Washington",output);
    assert(strcmp(output,"W252")==0);

    soundex("Wu",output);
    assert(strcmp(output,"W000")==0);

    soundex("DeSmet",output);
    assert(strcmp(output,"D253")==0);

    soundex("Gutierrez",output);
    assert(strcmp(output,"G362")==0);

    soundex("Pfister",output);
    assert(strcmp(output,"P236")==0);

    soundex("Jackson",output);
    assert(strcmp(output,"J250")==0);

    soundex("Tymczak",output);
    assert(strcmp(output,"T522")==0);

    soundex("Ashcraft",output);
    assert(strcmp(output,"A261")==0);
}
