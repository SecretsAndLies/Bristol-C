#include "specific.h"


// Create an empty BSA
bsa * bsa_init(void){
    bsa * b = acalloc(1,sizeof(bsa));
    b->array = acalloc(1, INITIAL_SIZE * sizeof(int_s));
    b->capacity = INITIAL_SIZE;
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
    if (b->size == b->capacity) {
        bsa_resize(b, b->capacity * RESIZE_FACTOR); 
    }
    if(b->array[indx].set==false){
        b->array[indx].num = d;
        return true;
    }
    b->array[indx].num = d;
    b->array[indx].set = true;
    b->size++;
    return true;
}

// Return pointer to data at element b[i]
// or NULL if element is unset
int* bsa_get(bsa* b, int indx){
    if(b->array[indx].set){
        return b->array[indx].num;
    }
    return NULL;
}

// Delete element at index indx 
bool bsa_delete(bsa* b, int indx){
    return false;
}

// Returns maximum index written to so far or
// -1 if no cells have been written to yet
int bsa_maxindex(bsa* b){
    if(b->size==0){
        return -1;
    }
    int max = 0;
    for(int i=0; i<b->capacity; i++){
        if(b->array->set){
            max = i;
        }
    }
    return max;
}

// Clears up all space used
bool bsa_free(bsa* b){
    free(b->array);
    b->array = NULL;
    free(b);
    return true;
}

// Allow a user-defined function to be applied to each (valid) value 
// in the array. The user defined 'func' is passed a pointer to an int,
// and maintains an accumulator of the result where required.
void bsa_foreach(void (*func)(int* p, int* n), bsa* b, int* acc){

}

// You'll this to test the other functions you write
void test(void){

}


// // Function to resize the array
void bsa_resize(bsa *bs, size_t new_capacity)
{
    int_s *temp = nremalloc(bs->array, new_capacity * sizeof(int_s));
    bs->array = temp;
    bs->capacity = new_capacity;
}

void* nremalloc(void* p, int bytes)
{
   void* n = realloc(p, bytes);
   if(n==NULL){
      on_error("Cannot malloc() space");
   }
   return n;
}

// ignored for this data structure.
bool bsa_tostring(bsa* b, char* str){
    return false;
}