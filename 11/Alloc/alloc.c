#include "specific.h"


    // 0 size 1: 0
    // 1 size 2: 1 2
    // 2 size 4: 3 4 5 6
    // 3 size 8: 7 8 9 10 11 12 13 14
    // 4 size 16: 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
    // 5 size 32: 31 ...
    // ...    // test_get_max_filled_row();

    // 29 size 536870912

// Create an empty BSA
// TODO ask neil how he feels about explicitly setting values.
bsa* bsa_init(void)
{
    bsa* b = acalloc(1, sizeof(bsa));
    for (int i = 0; i < BSA_ROWS; i++) {
        b->row_arr[i].r_len = get_length_of_row(i);
    }

    return b;
}

void test_bsa_init(void)
{
    bsa* b = bsa_init();
    assert(b);
    assert(b->total_size == 0);
    for (int i = 0; i < BSA_ROWS; i++) {
        assert(b->row_arr[i].int_arr == NULL);
        assert(b->row_arr[i].r_size == 0);
    }
    assert(b->row_arr[0].r_len == 1);
    assert(b->row_arr[1].r_len == 2);
    assert(b->row_arr[2].r_len == 4);
    assert(b->row_arr[3].r_len == 8);
    assert(b->row_arr[4].r_len == 16);
    assert(b->row_arr[5].r_len == 32);
    assert(b->row_arr[6].r_len == 64);
    assert(b->row_arr[7].r_len == 128);
    assert(b->row_arr[8].r_len == 256);
    assert(b->row_arr[29].r_len == 536870912);

    bsa_free(b);
}

// Set element at index indx with value d i.e. b[i] = d;
// May require an allocation if it's the first element in
// that row
bool bsa_set(bsa* b, int indx, int d) {
    if (b == NULL) {
        return false;
    }

    if (is_invalid_index(indx)) {
        return false;
    }

    int row_i = get_row_from_index(indx);

    if (b->row_arr[row_i].r_size == 0) {
        int len = b->row_arr[row_i].r_len;
        int size = sizeof(int_s);
        b->row_arr[row_i].int_arr = acalloc(len, size);
    }

    // increment the counter only if the existing value
    // isn't set.
    int col_i = get_col_within_row(row_i, indx);
    bool num_set = b->row_arr[row_i].int_arr[col_i].set;
    if (!num_set) {
        b->row_arr[row_i].r_size++;
        b->total_size++;
    }

    // finally, we actually set the int s struct.
    b->row_arr[row_i].int_arr[col_i].num = d;
    b->row_arr[row_i].int_arr[col_i].set = true;

    return true;
}

void test_bsa_set(void)
{
    bsa* b = bsa_init();
    bsa_set(b, 0, 4);
    assert(b->total_size == 1);
    assert(b->row_arr[0].r_size == 1);
    assert(b->row_arr[1].r_size == 0);
    assert(b->row_arr[0].int_arr[0].num == 4);
    assert(b->row_arr[0].int_arr[0].set == true);

    // test adding another number in the same pos, size
    // shouldn't increase
    bsa_set(b, 0, 4);
    assert(b->total_size == 1);
    assert(b->row_arr[0].r_size == 1);

    // test adding a number in a different row and check
    // that in between row is still 0
    bsa_set(b, 3, 9);
    assert(b->total_size == 2);
    assert(b->row_arr[2].int_arr[0].num == 9);
    assert(b->row_arr[2].int_arr[0].set == true);
    assert(b->row_arr[1].int_arr == 0);
    assert(b->row_arr[2].r_size == 1);

    // test adding values (overwriting existing)
    for (int i = 0; i < 10000; i++) {
        bsa_set(b, i, i);
    }
    assert(b->total_size == 10000);

    assert(bsa_set(b, MAX_INDEX, 19));
    assert(*bsa_get(b, MAX_INDEX) == 19);

    assert(!bsa_set(b, MAX_INDEX + 1, 19));

    bsa_free(b);
}

int get_length_of_row(int row)
{
    // same as 2 ^ row
    return 1<<row;
}

void test_get_length_of_row(void)
{
    assert(get_length_of_row(0) == 1);
    assert(get_length_of_row(1) == 2);
    assert(get_length_of_row(2) == 4);
    assert(get_length_of_row(3) == 8);
    assert(get_length_of_row(4) == 16);
    assert(get_length_of_row(5) == 32);
    assert(get_length_of_row(6) == 64);
}

int get_col_within_row(int row, int index)
{
    // avoids edge cases and zero issues
    int n = index + 1;
    // this is the same as row^2
    return n - (1 << row);
}

void test_get_col_within_row(void)
{
    assert(get_col_within_row(0, 0) == 0);

    assert(get_col_within_row(1, 1) == 0);
    assert(get_col_within_row(1, 2) == 1);

    assert(get_col_within_row(2, 3) == 0);
    assert(get_col_within_row(2, 4) == 1);
    assert(get_col_within_row(2, 5) == 2);
    assert(get_col_within_row(2, 6) == 3);

    assert(get_col_within_row(3, 7) == 0);
    assert(get_col_within_row(3, 8) == 1);
    assert(get_col_within_row(3, 9) == 2);
    assert(get_col_within_row(3, 10) == 3);
    assert(get_col_within_row(3, 11) == 4);
    assert(get_col_within_row(3, 12) == 5);
    assert(get_col_within_row(3, 13) == 6);
    assert(get_col_within_row(3, 14) == 7);

    assert(get_col_within_row(4, 15) == 0);
    assert(get_col_within_row(4, 16) == 1);
    assert(get_col_within_row(4, 17) == 2);
    assert(get_col_within_row(4, 18) == 3);
    assert(get_col_within_row(4, 19) == 4);
    assert(get_col_within_row(4, 20) == 5);
    assert(get_col_within_row(4, 21) == 6);
    assert(get_col_within_row(4, 22) == 7);
    assert(get_col_within_row(4, 23) == 8);
    assert(get_col_within_row(4, 24) == 9);
    assert(get_col_within_row(4, 25) == 10);
    assert(get_col_within_row(4, 26) == 11);
    assert(get_col_within_row(4, 27) == 12);
    assert(get_col_within_row(4, 28) == 13);
    assert(get_col_within_row(4, 29) == 14);
    assert(get_col_within_row(4, 30) == 15);
}

int get_row_from_index(int indx)
{
    int n = indx + 1;
    int row = 0;
    while (n > 1) {
        n = n >> 1;
        row++;
    }
    return row;
}

void test_get_row_from_index(void)
{
    assert(get_row_from_index(4) == 2);
    assert(get_row_from_index(12) == 3);
    assert(get_row_from_index(5) == 2);
    assert(get_row_from_index(12) == 3);
    assert(get_row_from_index(15) == 4);
    assert(get_row_from_index(30) == 4);
    assert(get_row_from_index(31) == 5);
    assert(get_row_from_index(0) == 0);
    assert(get_row_from_index(1) == 1);
    assert(get_row_from_index(2) == 1);
    assert(get_row_from_index(3) == 2);
}

void test_is_invalid_index(void)
{
    assert(is_invalid_index(5) == false);
    assert(is_invalid_index(0) == false);
    assert(is_invalid_index(-1));
    assert(is_invalid_index(MAX_INDEX - 1) == false);
    assert(is_invalid_index(MAX_INDEX) == false);
    assert(is_invalid_index(MAX_INDEX + 1));
}

bool is_invalid_index(int indx)
{
    if (indx > MAX_INDEX) {
        return true;
    }
    if (indx < 0) {
        return true;
    }
    return false;
}

// Return pointer to data at element b[i]
// or NULL if element is unset, or part of a row that hasn't been allocated.
int* bsa_get(bsa* b, int indx)
{
    if (b == NULL) {
        return NULL;
    }
    if (is_invalid_index(indx)) {
        return NULL;
    }
    int row_i = get_row_from_index(indx);
    int col_i = get_col_within_row(row_i, indx);
    if (b->row_arr[row_i].r_size == 0) {
        return NULL;
    }
    if (b->row_arr[row_i].int_arr[col_i].set == false) {
        return NULL;
    }
    return &(b->row_arr[row_i].int_arr[col_i].num);
}

void test_bsa_get(void)
{
    bsa* b = bsa_init();
    bsa_set(b, 0, 10);
    bsa_set(b, 1, 11);
    bsa_set(b, 2, 12);
    bsa_set(b, 3, 13);
    bsa_set(b, 4, 14);

    int* p = bsa_get(b, 0);
    assert(*p = 10);
    p = bsa_get(b, 1);
    assert(*p = 11);
    p = bsa_get(b, 2);
    assert(*p = 12);
    p = bsa_get(b, 3);
    assert(*p = 13);
    p = bsa_get(b, 4);
    assert(*p = 14);
    bsa_free(b);

    bsa* c = bsa_init();

    for (int i = 0; i < 10000; i++) {
        bsa_set(c, i, i - 2);
    }
    for (int i = 0; i < 10000; i++) {
        assert(*bsa_get(c, i) == i - 2);
    }
    bsa_free(c);
}

// Delete element at index indx - forces a shrink
// if that was the only cell in the row occupied.
bool bsa_delete(bsa* b, int indx)
{
    if (b == NULL) {
        return false;
    }
    if (is_invalid_index(indx)) {
        return NULL;
    }
    int row_i = get_row_from_index(indx);
    int col_i = get_col_within_row(row_i, indx);

    if (b->row_arr[row_i].r_size == 0) {
        // int_arr has already been freed.
        return false;
    }

    if (b->row_arr[row_i].int_arr[col_i].set == false) {
        // no element to delete - do nothing.
        return false;
    }
    b->row_arr[row_i].r_size--;
    b->total_size--;
    b->row_arr[row_i].int_arr[col_i].set = false;
    // if this was the last elemnt, free int_arr
    if (b->row_arr[row_i].r_size == 0) {
        free(b->row_arr[row_i].int_arr);
        b->row_arr[row_i].int_arr = NULL;
    }
    return true;
}

void test_bsa_delete(void)
{
    bsa* b = bsa_init();
    bsa_set(b, 0, 10);
    bsa_set(b, 1, 11);
    bsa_set(b, 2, 12);
    bsa_set(b, 3, 13);
    bsa_set(b, 4, 14);

    assert(b->total_size == 5);
    bsa_delete(b, 0);
    assert(b->total_size == 4);
    assert(b->row_arr[0].int_arr == NULL);
    // check that deleting the same element twice doesn't
    // decrement size
    bsa_delete(b, 0);
    assert(b->total_size == 4);

    // delete one element from a row. Check still there
    bsa_delete(b, 1);
    assert(b->row_arr[1].int_arr);
    assert(b->total_size == 3);

    // delete second element, row should now be gone.
    bsa_delete(b, 2);
    assert(b->row_arr[1].int_arr == NULL);
    assert(b->total_size == 2);

    bsa_free(b);
}

// Returns maximum index written to so far or
// -1 if no cells have been written to yet
// TODO optimize?
int bsa_maxindex(bsa* b)
{
    if (b == NULL) {
        return -1;
    }
    if (b->total_size == 0) {
        return -1;
    }

    int max_row = get_max_filled_row(b);

    int row_len = b->row_arr[max_row].r_len;
    int max_index = 0;
    for (int i = 0; i < row_len; i++) {
        if (b->row_arr[max_row].int_arr[i].set == true) {
            max_index = i;
        }
    }

    // convert arr index back into the orig index
    int orig_index =
        get_orig_index_from_arr_index(max_row, max_index);

    return orig_index;
}

int get_max_filled_row(bsa* b)
{
    int max_row = 0;
    for (int i = 0; i < BSA_ROWS; i++) {
        if (b->row_arr[i].r_size != 0) {
            max_row = i;
        }
    }
    return max_row;
}

void test_get_max_filled_row(void)
{
    bsa* b = bsa_init();

    bsa_set(b, 0, 0);
    assert(get_max_filled_row(b) == 0);
    bsa_set(b, 1, 10);
    assert(get_max_filled_row(b) == 1);
    bsa_set(b, 2, 10);
    assert(get_max_filled_row(b) == 1);
    bsa_set(b, 3, 10);
    assert(get_max_filled_row(b) == 2);
    bsa_set(b, 4, 10);
    assert(get_max_filled_row(b) == 2);
    bsa_delete(b, 3);
    assert(get_max_filled_row(b) == 2);
    bsa_delete(b, 4);
    assert(get_max_filled_row(b) == 1);

    bsa_free(b);
}

void test_bsa_maxindex(void)
{
    bsa* b = bsa_init();
    bsa_set(b, 0, 0);
    assert(bsa_maxindex(b) == 0);
    bsa_set(b, 1, 10);
    assert(bsa_maxindex(b) == 1);
    bsa_set(b, 2, 10);
    assert(bsa_maxindex(b) == 2);
    bsa_set(b, 3, 10);
    assert(bsa_maxindex(b) == 3);
    bsa_set(b, 4, 10);
    assert(bsa_maxindex(b) == 4);

    bsa_delete(b, 0);
    assert(bsa_maxindex(b) == 4);
    bsa_delete(b, 3);
    assert(bsa_maxindex(b) == 4);
    bsa_delete(b, 4);
    assert(bsa_maxindex(b) == 2);

    bsa_delete(b, 4);

    bsa_free(b);
}

int get_orig_index_from_arr_index(int row_index, int col_index)
{
    int sum = 0;
    int i = 0;
    while (i < row_index) {
        sum += get_length_of_row(i);
        i++;
    }
    return sum + col_index;
}

void test_get_orig_index_from_arr_index(void)
{
    assert(get_orig_index_from_arr_index(0, 0) == 0);
    assert(get_orig_index_from_arr_index(1, 0) == 1);
    assert(get_orig_index_from_arr_index(1, 1) == 2);

    assert(get_orig_index_from_arr_index(2, 0) == 3);
    assert(get_orig_index_from_arr_index(2, 1) == 4);
    assert(get_orig_index_from_arr_index(2, 2) == 5);
    assert(get_orig_index_from_arr_index(2, 3) == 6);

    assert(get_orig_index_from_arr_index(3, 0) == 7);
    assert(get_orig_index_from_arr_index(3, 1) == 8);
    assert(get_orig_index_from_arr_index(3, 2) == 9);
    assert(get_orig_index_from_arr_index(3, 3) == 10);
    assert(get_orig_index_from_arr_index(3, 4) == 11);
    assert(get_orig_index_from_arr_index(3, 5) == 12);
    assert(get_orig_index_from_arr_index(3, 6) == 13);
    assert(get_orig_index_from_arr_index(3, 7) == 14);

    assert(get_orig_index_from_arr_index(4, 0) == 15);
    assert(get_orig_index_from_arr_index(4, 1) == 16);
    assert(get_orig_index_from_arr_index(4, 2) == 17);
    assert(get_orig_index_from_arr_index(4, 3) == 18);
    assert(get_orig_index_from_arr_index(4, 4) == 19);
    assert(get_orig_index_from_arr_index(4, 5) == 20);
    assert(get_orig_index_from_arr_index(4, 6) == 21);
    assert(get_orig_index_from_arr_index(4, 7) == 22);
}

// Returns stringified version of structure
// Each row has its elements printed between {}, up to the maximum index.
// Rows after the maximum index are ignored.
// todo confirm use of sprintf (snprintf better?)
// todo rewrite (length)
bool bsa_tostring(bsa* b, char* str)
{
    if (b == NULL) {
        return false;
    }
    str[0] = '\0';
    if (b->total_size == 0) {
        strcpy(str, "");
        return true;
    }
    // todo, you might want to store max_row in the struct
    // itself.
    int max_row = get_max_filled_row(b);
    for (int r = 0; r <= max_row; r++) {
        build_row_string(str, r, b);
    }
    return true;
}

void build_row_string(char *str, int r, bsa * b)
{
    append_string(str,"{",LISTSTRLEN);
    if(b->row_arr[r].int_arr && b->row_arr[r].r_size>0){
        int count = 0;
        int r_len = b->row_arr[r].r_len;
            for(int c=0; c<r_len; c++){
                if(b->row_arr[r].int_arr[c].set){
                    count++;
                    int index = get_orig_index_from_arr_index(r,c);
                    int value = b->row_arr[r].int_arr[c].num;
                    char temp_string[LISTSTRLEN] = "";
                    sprintf(temp_string, "[%i]=%i", index, value);
                    append_string(str, temp_string, LISTSTRLEN);
                    if((b->row_arr[r].r_size)>count){
                        append_string(str, " ", LISTSTRLEN);
                    }
                }
            }
        }
    append_string(str, "}", LISTSTRLEN);
}

void append_string(char *dest, const char *source, size_t destSize)
{
    if (strlen(dest) + strlen(source) + 1 <= destSize) {
        strcat(dest, source);
    } else {
        on_error("Error, string too long.");
    }
}

void test_append_string(void)
{
    char str[LISTSTRLEN] = "";
    append_string(str, "test", LISTSTRLEN);
    assert(strcmp(str, "test") == 0);
    append_string(str, "2test", LISTSTRLEN);
    assert(strcmp(str, "test2test") == 0);
    append_string(str, "3test", LISTSTRLEN);
    assert(strcmp(str, "test2test3test") == 0);
}

void test_bsa_tostring(void)
{
    char str[LISTSTRLEN] = "";
    bsa* b = bsa_init();
    assert(bsa_set(b, 0, 0));
    assert(bsa_tostring(b, str));
    assert(strcmp(str, "{[0]=0}") == 0);
    assert(bsa_set(b, 1, 1));
    assert(bsa_tostring(b, str));
    assert(strcmp(str, "{[0]=0}{[1]=1}") == 0);
    assert(bsa_set(b, 2, 2));
    assert(bsa_tostring(b, str));
    assert(strcmp(str, "{[0]=0}{[1]=1 [2]=2}") == 0);
    assert(bsa_set(b, 10, 10));
    assert(bsa_tostring(b, str));
    assert(strcmp(str, "{[0]=0}{[1]=1 [2]=2}{}{[10]=10}") == 0);
    bsa_free(b);
}

// TODO test all functions with a blank value in the first
// row.

// Clears up all space used
// no tests exist for this function, memory leaks are
// detected using valgrind and sanitizer.
bool bsa_free(bsa* b){
    if (b == NULL) {
        return false;
    }
    for (int r = 0; r < BSA_ROWS; r++) {
        if (b->row_arr[r].int_arr) {
            free(b->row_arr[r].int_arr);
            b->row_arr[r].int_arr = NULL;
        }
    }

    free(b);
    return true;
}


// Allow a user-defined function to be applied to each (valid) value 
// in the array. The user defined 'func' is passed a pointer to an int,
// and maintains an accumulator of the result where required.
void bsa_foreach(void (*func)(int* p, int* n), bsa* b, int* acc)
{
    int max_row = get_max_filled_row(b);
    for (int r=0; r<=max_row; r++){
            if(b->row_arr[r].int_arr && b->row_arr[r].r_size>0){
                int r_len = b->row_arr[r].r_len;
                    for(int c=0; c<r_len; c++){
                        if(b->row_arr[r].int_arr[c].set){
                            func(&(b->row_arr[r].int_arr[c].num), acc);
                        }
                    }
                }
            }
}

// todo WRITE
void test_bsa_foreach(void)
{
    bsa* b = bsa_init();
    bsa_set(b, 1, 1);
    bsa_set(b, 2, 2);
    bsa_set(b, 3, 3);
    bsa_set(b, 35, 35);
    bsa_set(b, 36, 36);
    bsa_set(b, 37, 37);
    bsa_set(b, 38, 38);
    char str[LISTSTRLEN] = "";
    bsa_tostring(b, str);
    assert(strcmp(str,
                  "{}{[1]=1 [2]=2}{[3]=3}{}{}{[35]=35 "
                  "[36]=36 [37]=37 [38]=38}") == 0);
    // not used but required for function header.
    int acc = 0;
    bsa_foreach(add, b, &acc);
    bsa_tostring(b, str);
    assert(strcmp(str,
                  "{}{[1]=2 [2]=3}{[3]=4}{}{}{[35]=36 "
                  "[36]=37 [37]=38 [38]=39}") == 0);

    bsa_free(b);
}

void add(int* p, int* n)
{
    // Need to use n to switch off warnings :-(
    *n = 0;
    *p = *p + 1;
}

void test(void){
    test_is_invalid_index();
    test_bsa_init();
    test_get_row_from_index();
    test_get_col_within_row();
    test_get_length_of_row();
    test_get_orig_index_from_arr_index();
    test_bsa_set();
    test_bsa_get();
    test_bsa_delete();
    test_get_max_filled_row();
    test_bsa_maxindex();
    test_append_string();
    test_bsa_tostring();
    test_bsa_foreach();
}


void* acalloc(int n, size_t size)
{
    void* v = calloc(n, size);
    if (v == NULL) {
        on_error("Cannot calloc() space");
    }
    return v;
}

void on_error(const char* s)
{
    fprintf(stderr, "%s\n", s);
    exit(EXIT_FAILURE);
}

