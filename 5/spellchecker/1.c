#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define BIGSTR 100
#define NUMWORDS 129000

void addToIndex(char str_arr[NUMWORDS][BIGSTR], int i, char str[BIGSTR]);
void swap(char str_arr[NUMWORDS][BIGSTR], int i, int j);
bool word_in_list(char * word, char str_arr[NUMWORDS][BIGSTR], int len);
int build_and_sort_arr(char str_arr[NUMWORDS][BIGSTR], FILE * fp);
void test(void);
bool b_word_in_list(char * word, char str_arr[NUMWORDS][BIGSTR], int len);

int main (int argc, char **argv)
{
    test();
    if (argc!=2)
    {
        fprintf(stderr, "%s", "Wrong number of args. \n");
        exit(1);
    }

    FILE * fp = fopen(argv[1], "r");
    if (fp==NULL)
    {
        fprintf(stderr, "%s", "Cant open file. \n");
        exit(1);
    }

// avoid stack issues with static you could also str_arr = malloc(NUMWORDS * BIGSTR).
    static char str_arr[NUMWORDS][BIGSTR];
    int len = build_and_sort_arr(str_arr, fp);
    fclose(fp);

    assert(word_in_list("a", str_arr, len));
    assert(word_in_list("xx", str_arr, len)==false);
    assert(b_word_in_list("a", str_arr, len));
    assert(b_word_in_list("xx", str_arr, len)==false);
     
    
    return 0;
}

void test(void)
{
}

bool b_word_in_list(char * word, char str_arr[NUMWORDS][BIGSTR], int len)
{
    int r = 0;
    int l = len-1;
    while (l<=r)
    {
        int m = (l+r)/ 2;
        int dis = strcmp(word, str_arr[m]);
        if(dis==0){
            return true;
        }
        if(dis<0)
        {
            r=m-1;
        }
        if(dis>0)
        {
            l=m+1;
        }
    }
return false;
}

bool word_in_list(char * word, char str_arr[NUMWORDS][BIGSTR], int len)
{
    for (int i=0; i<len; i++)
    {
        if(strcmp(str_arr[i],word)==0){
            return true;
        }
    }
    return false;
}

int build_and_sort_arr(char str_arr[NUMWORDS][BIGSTR], FILE * fp)
{
    char str[BIGSTR];

    int len = 0;

    while(fscanf(fp, "%s", str)==1)
    {
        addToIndex(str_arr, len, str);
        len++;
       
       // if current string is less than left_string, swap.
       // but we can stop doing this when we get to the last item in
       // (because there's nothing to swap with.)
        int i=len-1;
        while(i>0 && (strcmp(str_arr[i], str_arr[i-1])<0))
        {
            swap(str_arr, i, i-1);
            i--;
        }
  
    }
    return len;
}


void swap(char str_arr[NUMWORDS][BIGSTR], int i, int j)
{
    char temp[BIGSTR];
    strcpy(temp, str_arr[i]);
    strcpy(str_arr[i], str_arr[j]);
    strcpy(str_arr[j], temp);
}

void addToIndex(char str_arr[NUMWORDS][BIGSTR], int i, char str[BIGSTR])
{
    strcpy(str_arr[i], str);
}
