#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIGSTR 100
#define NUMWORDS 129000

void addToIndex(char str_arr[NUMWORDS][BIGSTR], int i, char str[BIGSTR]);

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
    char str_arr[NUMWORDS][BIGSTR];
    int len = 0;
    addToIndex(str_arr, len, "test");

/*    while(fgets(str, BIGSTR, fp)!=NULL)
    {
        addToIndex(str_arr, len, str);
        len++;
    }
  */  
    printf("%s", str_arr[0]);

    fclose(fp);

    return 0;
}

void addToIndex(char str_arr[NUMWORDS][BIGSTR], int i, char str[BIGSTR])
{
    strcpy(str_arr[i], str);
}
