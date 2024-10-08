#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define BIGSTR 100
#define NUMWORDS 129000

typedef struct Node{
    char word[BIGSTR];
    struct Node* next;
} Node;

void printllist(Node * st);
void addToIndex(char str_arr[NUMWORDS][BIGSTR], int i, char str[BIGSTR]);
void swap(char str_arr[NUMWORDS][BIGSTR], int i, int j);
bool word_in_list(char * word, char str_arr[NUMWORDS][BIGSTR], int len);
int build_and_sort_arr(char str_arr[NUMWORDS][BIGSTR], FILE * fp);
void test(void);
bool b_word_in_list(char * word, char str_arr[NUMWORDS][BIGSTR], int len);
void add_to_linked_list(Node * tail, char * word, Node * head);
bool word_in_llist(char * word, Node* start);

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

    // build the linked list.
    Node *p;
    Node *start;
    start = p = calloc(1, sizeof(Node));
    assert(p); // check p was correctly malloc'd.

    char str[BIGSTR];
    if(fscanf(fp, "%s", str)!=1){
        exit(1);
    }
    strcpy(start->word, str);
    
    while(fscanf(fp, "%s", str)==1)
    {
        add_to_linked_list(p, str, start);
        p = p->next;
    }

    printllist(start);

    // int len = build_and_sort_arr(str_arr, fp);
    fclose(fp);

    //assert(word_in_llist("a", start));
    //assert(word_in_llist("xx", start)==false);
     
    return 0;
}


void printllist(Node * st)
{
    while (st != NULL){
        printf("%s ", st->word);
        st = st->next;
    };
    printf("\n");
}

/* adds to the place in the linked list depending on alphabetical order */
void add_to_linked_list(Node * tail, char * word, Node * head)
{
    // look through linked list.
    // If 
    Node * curr = head;
    while (curr != NULL){
        if (curr->next == NULL){
            // you're at the end, so do a normal add.
            // TODO: adapt this so that it adds either before or after, depending on what's needed. (this adds after.)
            tail->next = calloc(1, sizeof(Node));
            tail = tail->next;
            assert(tail);
            strcpy(tail->word, word);
            return;
        }
        // if next word is after word alphabetically (or is this letter)
        if(strcmp(curr->next->word,word)>=0)
        {
            Node * new = calloc(1, sizeof(Node));
            strcpy(new->word, word);
            new->next = curr->next;
            curr->next = new;
            return;
        }
    }
    }

void test(void)
{
}

bool b_word_in_list(char * word, char str_arr[NUMWORDS][BIGSTR], int len)
{
    int r = len-1;
    int l = 0;
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
