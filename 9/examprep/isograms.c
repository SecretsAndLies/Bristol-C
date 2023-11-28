#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
int isogram(char *s);
int main(int argc, char * argv[])
{
assert(isogram("programming") == 0);
assert(isogram("housewarmings") == 0);
assert(isogram("abductions") == 1);
assert(isogram("housewarming") == 1);
assert(isogram("hydromagnetics") == 1);
assert(isogram("uncopyrightable") == 1);

FILE * f = fopen(argv[1], "r");
char line[100];
int lon =0;
char longest[100];

while (fgets(line, sizeof(line), f))
{
    if(strlen(line)>lon){
        strcpy(longest, line);
    }
}

printf("%s (%i)", longest, lon);

fclose(f);


return 0;
}

int isogram(char *s)
{
    int hist[26] = {0};
    for (int i=0; i<strlen(s); i++){
        hist[s[i]-'a']++;
    }
    for (int i=0; i<26; i++){
        if(hist[i]>1){
            return 0;
        }
    }
    return 1;
}


