#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIGSTR 100
#define NUMWORDS 129000

void addToIndex(char str_arr[NUMWORDS][BIGSTR], int i, char str[BIGSTR]);
void swap(char str_arr[NUMWORDS][BIGSTR], int i, int j);

int main (int argc, char **argv)
{
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

    char str[BIGSTR];
    // avoid stack issues with static you could also str_arr = malloc(NUMWORDS * BIGSTR).
    static char str_arr[NUMWORDS][BIGSTR];
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


    printf("%s \n", str_arr[0]);
    printf("%s \n", str_arr[1]);
    printf("%s \n", str_arr[2]);
    printf("%s \n", str_arr[3]);
    printf("%s \n", str_arr[4]);
    printf("%s \n", str_arr[5]);

    fclose(fp);

    return 0;
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
