#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define ALPHABET_LEN 26
#define FNAME "words_alpha.txt"
#define BIGSTR 100

void test(void);
bool isAnagram(char* w, char* cw);

int main(int argc, char* argv[])
{
    test();
    if (argc!=2){
        fprintf(stderr, "One word required.");
        exit(EXIT_FAILURE);
    }
    char* orig_word = argv[1];
    
    
    FILE* fp = fopen(FNAME, "r");
    if (fp==NULL){
        fprintf(stderr, "can't open file \n");
        exit(EXIT_FAILURE);
    }

    // stolen https://stackoverflow.com/questions/1835986/how-to-use-eof-to-run-through-a-text-file-in-c
    char compare_word[BIGSTR]; 
    while (fgets(compare_word, BIGSTR, fp)!=NULL){
        // overwriting the new line character with a null char. 
        // windows issues?
        compare_word[strlen(compare_word)-1] = '\0'; 
        if (isAnagram(orig_word, compare_word)){
            printf("%s \n", compare_word);
        }
    }   
    
    // if we've out of the loop, but aren't at EOF,
    // something's wrong
    if (feof(fp)==false){
        fprintf(stderr, "cant read file line \n");
        exit(EXIT_FAILURE);
    }
    
    fclose(fp);
   return 0;
}


// no cases for upper case or non alpha numerics.
bool isAnagram(char* w, char* cw)
{
    // wrong length
   if (strlen(w)!=strlen(cw))
   {
       return false;
   }

   // same word
    if (strcmp(w, cw)==0){
        return false;
    }

    // otherwise, create an array that counts the number
    // of characters eg: aaaa becomes: 4,0,0...
    int histogram_w[ALPHABET_LEN] = {0};
    int histogram_cw[ALPHABET_LEN] = {0};
   
    int len = strlen(w);
    for (int i=0; i<len; i++)
    {
        int w_i = w[i]-'a';
        int cw_i = cw[i]-'a';
        histogram_w[w_i]++; 
        histogram_cw[cw_i]++;
    }

    // then compare the histograms
    for (int i=0; i<ALPHABET_LEN; i++)
    {
        if(histogram_w[i]!=histogram_cw[i]){
            return false;
        }
    }

    return true;

}

void test(void)
{
    assert(isAnagram("bat", "battt")==false);
    assert(isAnagram("bat", "bat")==false);
    assert(isAnagram("bat", "tab"));
    assert(isAnagram("aaab", "aaba"));
    assert(isAnagram("sternaig", "sternaeg")==false); 
}
