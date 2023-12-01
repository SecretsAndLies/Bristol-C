#include "specific.h"


    // 0 size 1: 0
    // 1 size 2: 1 2
    // 2 size 4: 3 4 5 6
    // 3 size 8: 7 8 9 10 11 12 13 14
    // 4 size 16: 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
    // 5 size 32: 31 ...
    // ...    // test_get_max_row_which_contains_a_value();

    // 29 size 536870912

// Create an empty BSA
// TODO ask neil how he feels about explicitly setting values.
bsa* bsa_init(void)
{
    bsa * b = ncalloc(1,sizeof(bsa));
    b->total_size=0;

    return b;
}

void test_bsa_init(void)
{
    bsa * b = bsa_init();
    assert(b);
    assert(b->total_size==0);
    for (int i=0; i<BSA_ROWS; i++){
        assert(b->row_arr[i].int_arr==0);
        assert(b->row_arr[i].r_size==0);
    }
    bsa_free(b);
}


// Set element at index indx with value d i.e. b[i] = d;
// May require an allocation if it's the first element in that row
bool bsa_set(bsa* b, int indx, int d)
{   
    if(b==NULL){
        return false;
    }
    int row_index = get_row_from_index(indx);

    if(b->row_arr[row_index].r_size == 0){
        int length = get_length_of_row(row_index);
        b->row_arr[row_index].int_arr = ncalloc(length,sizeof(int_s));
    }

    // increment the counter only if the existing value isn't set.
    int col_index = get_col_within_row(row_index, indx);
    if(b->row_arr[row_index].int_arr[col_index].set==false){
        b->row_arr[row_index].r_size++;
        b->total_size++;
    }

    // finally, we actually set the int s struct.
    b->row_arr[row_index].int_arr[col_index].num = d;
    b->row_arr[row_index].int_arr[col_index].set = true;
    
    return true;
}

void test_bsa_set(void)
{
    bsa * b = bsa_init();
    bsa_set(b,0,4);
    assert(b->total_size==1);
    assert(b->row_arr[0].r_size==1);
    assert(b->row_arr[1].r_size==0);
    assert(b->row_arr[0].int_arr[0].num==4);
    assert(b->row_arr[0].int_arr[0].set==true);

    // test adding another number in the same pos, size shouldn't increase
    bsa_set(b,0,4);
    assert(b->total_size==1);

    // test adding a number in a different row and check that in between row is still 0
    bsa_set(b,3,9);
    assert(b->total_size==2);
    assert(b->row_arr[2].int_arr[0].num==9);
    assert(b->row_arr[2].int_arr[0].set==true);
    assert(b->row_arr[1].int_arr==0);

    bsa_free(b);
}

int get_length_of_row(int row)
{
    // 2 ^ row
    return 1<<row;
}

void test_get_length_of_row(void)
{
    assert(get_length_of_row(0)==1);
    assert(get_length_of_row(1)==2);
    assert(get_length_of_row(2)==4);
    assert(get_length_of_row(3)==8);
    assert(get_length_of_row(4)==16);
    assert(get_length_of_row(5)==32);
    assert(get_length_of_row(6)==64);
}

int get_col_within_row(int row, int index)
{
    int n = index+1; // avoids edge cases and zero issues.
    return n-(1<<row); // this is the same as row^2
}

void test_get_col_within_row(void)
{
    assert(get_col_within_row(0,0)==0);

    assert(get_col_within_row(1,1)==0);
    assert(get_col_within_row(1,2)==1);

    assert(get_col_within_row(2,3)==0);
    assert(get_col_within_row(2,4)==1);
    assert(get_col_within_row(2,5)==2);
    assert(get_col_within_row(2,6)==3);

    assert(get_col_within_row(3,7)==0);
    assert(get_col_within_row(3,8)==1);
    assert(get_col_within_row(3,9)==2);
    assert(get_col_within_row(3,10)==3);
    assert(get_col_within_row(3,11)==4);
    assert(get_col_within_row(3,12)==5);
    assert(get_col_within_row(3,13)==6);
    assert(get_col_within_row(3,14)==7);

    assert(get_col_within_row(4,15)==0);
    assert(get_col_within_row(4,16)==1);
    assert(get_col_within_row(4,17)==2);
    assert(get_col_within_row(4,18)==3);
    assert(get_col_within_row(4,19)==4);
    assert(get_col_within_row(4,20)==5);
    assert(get_col_within_row(4,21)==6);
    assert(get_col_within_row(4,22)==7);
    assert(get_col_within_row(4,23)==8);
    assert(get_col_within_row(4,24)==9);
    assert(get_col_within_row(4,25)==10);
    assert(get_col_within_row(4,26)==11);
    assert(get_col_within_row(4,27)==12);
    assert(get_col_within_row(4,28)==13);
    assert(get_col_within_row(4,29)==14);
    assert(get_col_within_row(4,30)==15);

}

int get_row_from_index(int indx)
{
    /*
    Make a copy of index called n and increase by one (to cover the edge cases)
    So for example, 0011 would become 0100
    then we bit shift left until only 1 is left
    and return that number. So 10 returns 3.
    */

    int n = indx+1;
    int row = 0;
    while (n>1){
            n = n>>1;
            row++;
    }
    return row;
}

void test_get_row_from_index(void)
{
    assert(get_row_from_index(4)==2);
    assert(get_row_from_index(12)==3);
    assert(get_row_from_index(5)==2);
    assert(get_row_from_index(12)==3);
    assert(get_row_from_index(15)==4);
    assert(get_row_from_index(30)==4);
    assert(get_row_from_index(31)==5);
    assert(get_row_from_index(0)==0);
    assert(get_row_from_index(1)==1);
    assert(get_row_from_index(2)==1);
    assert(get_row_from_index(3)==2);
}

// Return pointer to data at element b[i]
// or NULL if element is unset, or part of a row that hasn't been allocated.
int* bsa_get(bsa* b, int indx)
{
    if(b==NULL){
        return NULL;
    }
    int row_i = get_row_from_index(indx);
    int col_i = get_col_within_row(row_i,indx);
    if(b->row_arr[row_i].r_size==0){
        return NULL;
    }
    if(b->row_arr[row_i].int_arr[col_i].set==false){
        return NULL;
    }
    return &(b->row_arr[row_i].int_arr[col_i].num);
}

void test_bsa_get(void)
{
    bsa * b = bsa_init();
    bsa_set(b, 0, 10);
    bsa_set(b, 1, 11);
    bsa_set(b, 2, 12);
    bsa_set(b, 3, 13);
    bsa_set(b, 4, 14);

    int * p = bsa_get(b,0);
    assert(*p=10);
    p = bsa_get(b,1);
    assert(*p=11);
    p = bsa_get(b,2);
    assert(*p=12);
    p = bsa_get(b,3);
    assert(*p=13);
    p = bsa_get(b,4);
    assert(*p=14);
    bsa_free(b);

}

// Delete element at index indx - forces a shrink
// if that was the only cell in the row occupied.
bool bsa_delete(bsa* b, int indx)
{
    if(b==NULL){
        return false;
    }
    int row_i = get_row_from_index(indx);
    int col_i = get_col_within_row(row_i,indx);

    if(b->row_arr[row_i].int_arr==0){
        return false; // int_arr has already been freed.
    }

    if(b->row_arr[row_i].int_arr[col_i].set=false){
        return false; // no element to delete - do nothing.
    }
    b->row_arr[row_i].r_size--;
    b->total_size--;
    b->row_arr[row_i].int_arr[col_i].set=false;
    // if this was the last elemnt, free int_arr
    if(b->row_arr[row_i].r_size==0){
        free(b->row_arr[row_i].int_arr);
        b->row_arr[row_i].int_arr=0;
    }
    return true;

}

void test_bsa_delete(void)
{
    bsa * b = bsa_init();
    bsa_set(b, 0, 10);
    bsa_set(b, 1, 11);
    bsa_set(b, 2, 12);
    bsa_set(b, 3, 13);
    bsa_set(b, 4, 14);

    assert(b->total_size==5);
    bsa_delete(b,0);
    assert(b->total_size==4);
    assert(b->row_arr[0].int_arr==0);
    // check that deleting the same element twice doesn't decrement size
    bsa_delete(b,0);
    assert(b->total_size==4);

    // delete one element from a row. Check still there
    bsa_delete(b,1);
    assert(b->row_arr[1].int_arr);
    assert(b->total_size==3);

    // delete second element, row should now be gone.
    bsa_delete(b,2);
    assert(b->row_arr[1].int_arr==0);
    assert(b->total_size==2);

    bsa_free(b);
}

// Returns maximum index written to so far or
// -1 if no cells have been written to yet
    // TODO this could be optimized adding space. (Sam's approach)
int bsa_maxindex(bsa* b)
{
    if(b==NULL){
        return -1; 
    }
    if(b->total_size==0){
        return -1;
    }

    int max_row = get_max_row_which_contains_a_value(b);

    int row_len = get_length_of_row(max_row);
    int max_index = 0;
    for (int i=0; i<row_len; i++)
    {
        if(b->row_arr[max_row].int_arr[i].set==true){
            max_index=i;
        }
    }

    // convert arr index back into the orig index
    int orig_index = get_orig_index_from_arr_index(max_row, max_index);

    return orig_index;
}

// todo rename
int get_max_row_which_contains_a_value(bsa* b)
{
    int max_row = 0;
    for (int i=0; i<BSA_ROWS; i++)
    {
        if(b->row_arr[i].r_size!=0){
            max_row=i;
        }
    }
    return max_row;
}

// TODO write
void test_get_max_row_which_contains_a_value(void)
{

}

void test_bsa_maxindex(void)
{
   bsa * b = bsa_init();
   bsa_set(b,0,0);
   assert(bsa_maxindex(b)==0);
   bsa_set(b,1,10);
   assert(bsa_maxindex(b)==1);
   bsa_set(b,2,10);
   assert(bsa_maxindex(b)==2);
   bsa_set(b,3,10);
   assert(bsa_maxindex(b)==3);
   bsa_set(b,4,10);
   assert(bsa_maxindex(b)==4);

   bsa_delete(b,0);
   assert(bsa_maxindex(b)==4);
   bsa_delete(b,3);
   assert(bsa_maxindex(b)==4);
   bsa_delete(b,4);
   assert(bsa_maxindex(b)==2);

   bsa_delete(b,4);

   bsa_free(b);

}

int get_orig_index_from_arr_index(int row_index, int col_index)
{
    int sum = 0;
    int i = 0;
    while (i<row_index){
        sum+=get_length_of_row(i);
        i++;
    }
    return sum+col_index;
}

void test_get_orig_index_from_arr_index(void)
{
    assert(get_orig_index_from_arr_index(0,0)==0);
    assert(get_orig_index_from_arr_index(1,0)==1);
    assert(get_orig_index_from_arr_index(1,1)==2);
    
    assert(get_orig_index_from_arr_index(2,0)==3);
    assert(get_orig_index_from_arr_index(2,1)==4);
    assert(get_orig_index_from_arr_index(2,2)==5);
    assert(get_orig_index_from_arr_index(2,3)==6);

    assert(get_orig_index_from_arr_index(3,0)==7);
    assert(get_orig_index_from_arr_index(3,1)==8);
    assert(get_orig_index_from_arr_index(3,2)==9);
    assert(get_orig_index_from_arr_index(3,3)==10);
    assert(get_orig_index_from_arr_index(3,4)==11);
    assert(get_orig_index_from_arr_index(3,5)==12);
    assert(get_orig_index_from_arr_index(3,6)==13);
    assert(get_orig_index_from_arr_index(3,7)==14);

    assert(get_orig_index_from_arr_index(4,0)==15);
    assert(get_orig_index_from_arr_index(4,1)==16);
    assert(get_orig_index_from_arr_index(4,2)==17);
    assert(get_orig_index_from_arr_index(4,3)==18);
    assert(get_orig_index_from_arr_index(4,4)==19);
    assert(get_orig_index_from_arr_index(4,5)==20);
    assert(get_orig_index_from_arr_index(4,6)==21);
    assert(get_orig_index_from_arr_index(4,7)==22);
}

// Returns stringified version of structure
// Each row has its elements printed between {}, up to the maximum index.
// Rows after the maximum index are ignored.
bool bsa_tostring(bsa* b, char* str)
{
    return false;
}

void test_bsa_tostring(void)
{
    bsa b = bsa_init();
       assert(bsa_set(b, 0, 0));
   assert(bsa_set(b, 15, 15));


}

// Clears up all space used
bool bsa_free(bsa* b){
    if(b==NULL){
        return false;
    }
    // TODO also free all the inner structures.
    free(b);
    return true;
}

// Allow a user-defined function to be applied to each (valid) value 
// in the array. The user defined 'func' is passed a pointer to an int,
// and maintains an accumulator of the result where required.
void bsa_foreach(void (*func)(int* p, int* n), bsa* b, int* acc){

}

void test(void){
    test_bsa_init();
    test_get_row_from_index();
    test_get_col_within_row();
    test_get_length_of_row();
    test_get_orig_index_from_arr_index();
    test_bsa_set();
    test_bsa_get();
    test_bsa_delete();
    test_get_max_row_which_contains_a_value();
    test_bsa_maxindex();
}


void* ncalloc(int n, size_t size)
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