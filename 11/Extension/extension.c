#include "specific.h"


// Create an empty BSA
bsa * bsa_init(void){
    bsa * b = acalloc(1, sizeof(bsa));
    return b;

}

void* acalloc(int n, size_t size)
{
   void* v = calloc(n, size);
   if(v==NULL){
      on_error("Cannot calloc() space");
   }
   return v;
}

void on_error(const char* s)
{
   fprintf(stderr, "%s\n", s);
   exit(EXIT_FAILURE);
}

// Set element at index indx with value d i.e. b[i] = d;
bool bsa_set(bsa* b, int indx, int d){
    if(b==NULL){
        return false;
    }
    int bucket = hash(indx);

    // if that node already exists overwrite it.
    node *curr = b->buckets[bucket];
        while (curr) {
            if (curr->index == indx) {
                curr->num = d;
                return true;
            }
            curr = curr->next;
        }
    // otherwise, create a new node.
    node *new_node = acalloc(1, sizeof(node));
    new_node->index = indx;
    new_node->num = d;

    // this can be null.
    new_node->next = b->buckets[bucket]; 
    b->buckets[bucket] = new_node;

    return true;
}

// TODO think of an improved hash function.
int hash(int index) 
{
    return index % SIZE;
}

// Return pointer to data at element b[i]
// or NULL if element is unset
int* bsa_get(bsa* b, int indx)
{
    if(b==NULL){
        return false;
    }
    int bucket = hash(indx);
    node *curr = b->buckets[bucket];

    while (curr) {
        if (curr->index == indx) {
            return &(curr->num);
        }
        curr = curr->next;
    }

    return NULL;
}

// Delete element at index indx 
bool bsa_delete(bsa* b, int indx){
    if(b==NULL){
        return false;
    }

    int bucket = hash(indx);
    node *curr = b->buckets[bucket];
    node *prev = NULL;

    while (curr != NULL) {
        if (curr->index == indx) {
            // If it is the first node in the list
            if (prev == NULL) {
                // this can also set curr->next to null
                b->buckets[bucket] = curr->next;
            } 
            else {
                prev->next = curr->next;
            }
            free(curr);

            return true;
        }
        prev = curr;
        curr = curr->next;
    }

    // index not found
    return false;
}

// Returns maximum index written to so far or
// -1 if no cells have been written to yet
int bsa_maxindex(bsa* b){
    if(b==NULL){
        return -1;
    }
    int max = -1;
    for(int i = 0; i<SIZE; i++) {
        node *curr = b->buckets[i];

        while (curr) {
            if (curr->index > max) {
                max = curr->index;
            }
            curr = curr->next;
        }
    }
    return max;
}


// Clears up all space used
bool bsa_free(bsa* b){
    if(b==NULL){
        return false;
    }
    for (int i = 0; i < SIZE; i++) {
        node *curr = b->buckets[i];
        while (curr) {
            node *temp = curr;
            curr = curr->next;
            free(temp);
        }
    }

    free(b);
    return true;
}

// Allow a user-defined function to be applied to each (valid) value 
// in the array. The user defined 'func' is passed a pointer to an int,
// and maintains an accumulator of the result where required.
void bsa_foreach(void (*func)(int* p, int* n), bsa* b, int* acc){
    if(b==NULL){
        return false;
    }
    for (int i = 0; i < SIZE; i++) {
        node *curr = b->buckets[i];
        while (curr) {
            func(&(curr->num), acc);
            node *temp = curr;
            curr = curr->next;
        }
    }
}

// TODO: bash script that compares the output of ext and reg.

void test(void){

    assert(hash(0)==0);
    assert(hash(2)==2);
    assert(hash(4)==4);
    assert(hash(5)==5);
    assert(hash(15319)==0);
    assert(hash(15320)==1);

    char str[LISTSTRLEN] = "";

    // A NULL BSA has size zero
    assert(bsa_maxindex(NULL)==-1);
    assert(!bsa_tostring(NULL, str));

    // Initialise
    bsa* b = bsa_init();
    assert(b);
    assert(bsa_maxindex(b)==-1);
    assert(b->buckets[0]==NULL); 

    // Set some values
    assert(bsa_set(b, 0, 4));
    assert(bsa_maxindex(b)==0);
    // Reset existing value
    assert(bsa_set(b, 0, 0));
    assert(bsa_set(b, 15, 15));
    // todo add elements that test where things end up.

    // Get some values tbsa've already been set
    int* p = bsa_get(b, 0);
    assert(p);
    assert(*p == 0);
    p = bsa_get(b, 15);
    assert(p);
    assert(*p == 15);
    assert(b->buckets[15]->num==15);
    assert(b->buckets[15]->next==NULL);

    // Get an unset value 
    p = bsa_get(b, 1);
    assert(!p);
    p = bsa_get(b, 1024);
    assert(!p);

    assert(bsa_set(b, 100, 100));
    assert(bsa_maxindex(b)==100);
    assert(b->buckets[100]->num==100);
    assert(b->buckets[15]->next==NULL);
    p = bsa_get(b, 100);
    assert(p);
    assert(*p == 100);
    // make sure nothing has changed.
    assert(b->buckets[15]->num==15);
    assert(b->buckets[15]->next==NULL);


    // Let's do some deleting

    // Cell not not used, can't delete it
    assert(!bsa_delete(b, 99));
    // Cell is used, can delete it
    assert(b->buckets[0]->next==NULL);
    assert(bsa_delete(b, 100));
    assert(bsa_maxindex(b)==15);
    assert(b->buckets[100]==NULL);
    // Check it's gone
    p = bsa_get(b, 100);
    assert(!p);
    // Cell is used, can delete it
    assert(bsa_delete(b, 15));
    assert(b->buckets[15]==NULL);
    assert(bsa_maxindex(b)==0);
    p = bsa_get(b, 15);
    assert(!p);
    // Delete last element left
    assert(bsa_delete(b, 0));
    assert(b->buckets[0]==NULL);
    assert(bsa_maxindex(b)<0);
    p = bsa_get(b, 0);
    assert(!p);

    bsa_free(b);

    // // foreach - use it to compute product of numbers, and also to double each one
    // b = bsa_init();
    // bsa_set(b, 1, 1);
    // bsa_set(b, 2, 2);
    // bsa_set(b, 3, 3);
    // int acc = 1;
    // bsa_foreach(times, b, &acc);
    // // printf("acc %i \n", acc);
    // assert(acc==6);
    // // Double each numbers in array
    // assert(bsa_tostring(b, str));
    // assert(strcmp(str, "{}{[1]=1 [2]=2}{[3]=3}")==0);
    // bsa_foreach(twice, b, &acc);
    // assert(bsa_tostring(b, str));
    // assert(strcmp(str, "{}{[1]=2 [2]=4}{[3]=6}")==0);
    // bsa_free(b);

}

void times(int* p, int* n)
{
   *n = *n * *p;
}

void twice(int* p, int* n)
{
   // Need to use n to switch off warnings :-(
   *n = 0;
   *p = *p * 2;
}
   

// ignore this, it's just to switch of warnings.
bool bsa_tostring(bsa* b, char* str){
    if(b){
        *str = '\0';
        return true;
    }    
    return false;
}