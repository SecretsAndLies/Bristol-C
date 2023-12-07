#include "specific.h"

// Create an empty BSA
bsa* bsa_init(void) 
{
    bsa* b = acalloc(1, sizeof(bsa));
    for(int i = 0; i < BSA_ROWS; i++){
        row * r = &(b->row_arr[i]);
        r->r_len = get_row_len(i);
    }
    return b;
}

void test_bsa_init(void) 
{
    bsa* b = bsa_init();
    // check exists
    assert(b);
    // check calloc worked sucessfully
    assert(b->total_size == 0);
    for(int i = 0; i < BSA_ROWS; i++){
        assert(b->row_arr[i].int_arr == NULL);
        assert(b->row_arr[i].r_size == 0);
    }
    // check row length has been set.
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

bool is_invalid_input(bsa* b, int indx) 
{
    if(b == NULL){
        return true;
    }

    if(is_invalid_index(indx)){
        return true;
    }

    return false;
}

// Set element at index indx with value d i.e. b[i] = d;
// May require an allocation if it's the first element in
// that row
bool bsa_set(bsa* b, int indx, int d) 
{
    if(is_invalid_input(b, indx)){
        return false;
    }
    int row_i = get_row(indx);
    row * r = &(b->row_arr[row_i]);

    if(r->r_size == 0){
        int len = r->r_len;
        int size = sizeof(int_s);
        r->int_arr = acalloc(len, size);
    }
    int col_i = get_col(row_i, indx);
    bool num_set = r->int_arr[col_i].set;
    if(!num_set){
        r->r_size++;
        b->total_size++;
    }
    r->int_arr[col_i].num = d;
    r->int_arr[col_i].set = true;
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
    for(int i = 0; i < 10000; i++){
        bsa_set(b, i, i);
    }
    assert(b->total_size == 10000);

    assert(bsa_set(b, get_max_index(BSA_ROWS), 19));
    assert(*bsa_get(b, get_max_index(BSA_ROWS)) == 19);

    assert(!bsa_set(b, get_max_index(BSA_ROWS) + 1, 19));

    bsa_free(b);
}

int get_max_index(int row_count)
{
    return (1<<row_count)-OFFSET;
}

void test_get_max_index(void)
{
    assert(get_max_index(30)==1073741822);
}

void test_is_invalid_input(void) 
{
    bsa* b = bsa_init();
    assert(is_invalid_input(NULL, 0));
    assert(is_invalid_input(b, -1));
    assert(is_invalid_input(b, get_max_index(BSA_ROWS) + 1));
    assert(is_invalid_input(b, 0) == false);
    assert(is_invalid_input(b, 1) == false);
    assert(is_invalid_input(b, get_max_index(BSA_ROWS)) == false);
    bsa_free(b);
}

int get_row_len(int row) 
{
    // same as 2 ^ row
    return 1 << row;
}

void test_get_row_len(void) 
{
    assert(get_row_len(0) == 1);
    assert(get_row_len(1) == 2);
    assert(get_row_len(2) == 4);
    assert(get_row_len(3) == 8);
    assert(get_row_len(4) == 16);
    assert(get_row_len(5) == 32);
    assert(get_row_len(6) == 64);
}

int get_col(int row, int index) 
{
    // avoids edge cases and zero issues
    int n = index + 1;
    // this is the same as row^2
    return n - (1 << row);
}

void test_get_col(void) 
{
    assert(get_col(0, 0) == 0);

    assert(get_col(1, 1) == 0);
    assert(get_col(1, 2) == 1);

    assert(get_col(2, 3) == 0);
    assert(get_col(2, 4) == 1);
    assert(get_col(2, 5) == 2);
    assert(get_col(2, 6) == 3);

    assert(get_col(3, 7) == 0);
    assert(get_col(3, 8) == 1);
    assert(get_col(3, 9) == 2);
    assert(get_col(3, 10) == 3);
    assert(get_col(3, 11) == 4);
    assert(get_col(3, 12) == 5);
    assert(get_col(3, 13) == 6);
    assert(get_col(3, 14) == 7);

    assert(get_col(4, 15) == 0);
    assert(get_col(4, 16) == 1);
    assert(get_col(4, 17) == 2);
    assert(get_col(4, 18) == 3);
    assert(get_col(4, 19) == 4);
    assert(get_col(4, 20) == 5);
    assert(get_col(4, 21) == 6);
    assert(get_col(4, 22) == 7);
    assert(get_col(4, 23) == 8);
    assert(get_col(4, 24) == 9);
    assert(get_col(4, 25) == 10);
    assert(get_col(4, 26) == 11);
    assert(get_col(4, 27) == 12);
    assert(get_col(4, 28) == 13);
    assert(get_col(4, 29) == 14);
    assert(get_col(4, 30) == 15);
}

int get_row(int indx) 
{
    int n = indx + 1;
    int row = 0;
    while(n > 1){
        n = n >> 1;
        row++;
    }
    return row;
}

void test_get_row(void) 
{
    assert(get_row(4) == 2);
    assert(get_row(12) == 3);
    assert(get_row(5) == 2);
    assert(get_row(12) == 3);
    assert(get_row(15) == 4);
    assert(get_row(30) == 4);
    assert(get_row(31) == 5);
    assert(get_row(0) == 0);
    assert(get_row(1) == 1);
    assert(get_row(2) == 1);
    assert(get_row(3) == 2);
}

void test_is_invalid_index(void) 
{
    assert(is_invalid_index(5) == false);
    assert(is_invalid_index(0) == false);
    assert(is_invalid_index(-1));
    assert(is_invalid_index(get_max_index(BSA_ROWS) - 1) == false);
    assert(is_invalid_index(get_max_index(BSA_ROWS)) == false);
    assert(is_invalid_index(get_max_index(BSA_ROWS) + 1));
}

bool is_invalid_index(int indx) 
{
    if(BSA_ROWS<1){
        on_error("BSA_ROWS must be greater than 0.");
    }
    if(indx > get_max_index(BSA_ROWS)){
        return true;
    }
    if(indx < 0){
        return true;
    }
    return false;
}

// Return pointer to data at element b[i]
// or NULL if element is unset, or part of a row that hasn't
// been allocated.
int* bsa_get(bsa* b, int indx) 
{
    if(is_invalid_input(b, indx)){
        return false;
    }
    int row_i = get_row(indx);
    int col_i = get_col(row_i, indx);
    row * r = &(b->row_arr[row_i]);
    if(r->r_size == 0){
        return NULL;
    }
    if(r->int_arr[col_i].set == false){
        return NULL;
    }
    return &(r->int_arr[col_i].num);
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

    for(int i = 0; i < 10000; i++){
        bsa_set(c, i, i - 2);
    }
    for(int i = 0; i < 10000; i++){
        assert(*bsa_get(c, i) == i - 2);
    }
    bsa_free(c);
}

// Delete element at index indx - forces a shrink
// if that was the only cell in the row occupied.
bool bsa_delete(bsa* b, int indx) 
{
    if(is_invalid_input(b, indx)){
        return false;
    }
    int row_i = get_row(indx);
    int col_i = get_col(row_i, indx);
    row * r = &(b->row_arr[row_i]);

    if(r->r_size == 0){
        return false;
    }
    if(r->int_arr[col_i].set == false){
        return false;
    }
    r->r_size--;
    b->total_size--;
    r->int_arr[col_i].set = false;
    if(r->r_size == 0){
        free(r->int_arr);
        r->int_arr = NULL;
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
int bsa_maxindex(bsa* b) 
{
    if(b == NULL || b->total_size == 0){
        return -1;
    }
    int max_row = get_max_filled_row(b);
    row r = b->row_arr[max_row];
    int row_len = r.r_len;
    int max_index = 0;
    for(int i = 0; i < row_len; i++){
        if(r.int_arr[i].set == true){
            max_index = i;
        }
    }
    int orig_index = get_index(max_row, max_index);
    return orig_index;
}

int get_max_filled_row(bsa* b) 
{
    int max_row = 0;
    for(int i = 0; i < BSA_ROWS; i++){
        row r = b->row_arr[i];
        if(r.r_size != 0){
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
    assert(bsa_maxindex(NULL)==-1);
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

int get_index(int row, int col) 
{
    int sum = 0;
    int i = 0;
    while(i < row){
        sum += get_row_len(i);
        i++;
    }
    return sum + col;
}

void test_get_index(void) 
{
    assert(get_index(0, 0) == 0);
    assert(get_index(1, 0) == 1);
    assert(get_index(1, 1) == 2);

    assert(get_index(2, 0) == 3);
    assert(get_index(2, 1) == 4);
    assert(get_index(2, 2) == 5);
    assert(get_index(2, 3) == 6);

    assert(get_index(3, 0) == 7);
    assert(get_index(3, 1) == 8);
    assert(get_index(3, 2) == 9);
    assert(get_index(3, 3) == 10);
    assert(get_index(3, 4) == 11);
    assert(get_index(3, 5) == 12);
    assert(get_index(3, 6) == 13);
    assert(get_index(3, 7) == 14);

    assert(get_index(4, 0) == 15);
    assert(get_index(4, 1) == 16);
    assert(get_index(4, 2) == 17);
    assert(get_index(4, 3) == 18);
    assert(get_index(4, 4) == 19);
    assert(get_index(4, 5) == 20);
    assert(get_index(4, 6) == 21);
    assert(get_index(4, 7) == 22);
}

// Returns stringified version of structure
// Each row has its elements printed between{}, up to the
// maximum index. Rows after the maximum index are ignored.
bool bsa_tostring(bsa* b, char* str) 
{
    if(b == NULL || str == NULL){
        return false;
    }
    str[0] = '\0';
    if(b->total_size == 0){
        strcpy(str, "");
        return true;
    }
    int max_row = get_max_filled_row(b);
    for(int r = 0; r <= max_row; r++){
        build_row_string(str, r, b);
    }
    return true;
}

void build_row_string(char* str, int r, bsa* b) 
{
    strcat(str, "{");
    row row = b->row_arr[r];
    if(row.int_arr && row.r_size > 0){
        int count = 0;
        int r_len = row.r_len;
        for(int c = 0; c < r_len; c++){
            if(row.int_arr[c].set){
                count++;
                int indx = get_index(r, c);
                int value = row.int_arr[c].num;
                char col_str[LISTSTRLEN] = "";
                sprintf(col_str, "[%i]=%i", indx, value);
                strcat(str, col_str);
                if((row.r_size) > count){
                    strcat(str, " ");
                }
            }
        }
    }
    strcat(str, "}");
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

// Clears up all space used
// no tests exist for this function, memory leaks are
// detected using valgrind and sanitizer.
bool bsa_free(bsa* b) 
{
    if(b == NULL){
        return false;
    }
    for(int i = 0; i < BSA_ROWS; i++){
        row r = b->row_arr[i];
        if(r.int_arr){
            free(r.int_arr);
            r.int_arr = NULL;
        }
    }
    free(b);
    return true;
}

// Allow a user-defined function to be applied to each
// (valid) value in the array. The user defined 'func' is
// passed a pointer to an int, and maintains an accumulator
// of the result where required.
void bsa_foreach(void (*func)(int* p, int* n), bsa* b, int* acc) 
{
    if(!b || !func || !acc){
        return;
    }
    int max_row = get_max_filled_row(b);
    for(int i = 0; i <= max_row; i++){
        row r = b->row_arr[i];
        if(r.int_arr && r.r_size > 0){
            int r_len = r.r_len;
            for(int j = 0; j < r_len; j++){
                bool num_set = r.int_arr[j].set;
                if(num_set){
                    func(&(r.int_arr[j].num), acc);
                }
            }
        }
    }
}

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

    // pass null pointers and test that it doesn't crash
    bsa_foreach(NULL, b, &acc);
    bsa_foreach(add, NULL, &acc);
    bsa_foreach(add, b, NULL);

    bsa_free(b);
}

void test(void) 
{
    test_is_invalid_index();
    test_bsa_init();
    test_get_row();
    test_get_col();
    test_get_row_len();
    test_get_index();
    test_bsa_set();
    test_bsa_get();
    test_bsa_delete();
    test_get_max_filled_row();
    test_bsa_maxindex();
    test_bsa_tostring();
    test_bsa_foreach();
    test_is_invalid_input();

    // integration tests for set get and delete
    // add random values sparsely
    bsa* b = bsa_init();
    for(int i = 3; i <= 393216; i *= 2){
        bsa_set(b, i, i);
        assert(*bsa_get(b, i) == i);
        assert(bsa_delete(b, i));
        assert((bsa_get(b, i)) == NULL);

        // try to delete again
        assert(bsa_delete(b, i) == false);

        // set three times (overwrite)
        bsa_set(b, i, i + 1);
        bsa_set(b, i, i + 2);
        bsa_set(b, i, i + 3);
        assert(*bsa_get(b, i) == i + 3);
    }
    assert(b->total_size > 0);

    char str[LISTSTRLEN];
    bsa_tostring(b, str);
    // test that the first and second rows are still blank.
    assert(b->row_arr[0].r_size == 0);
    assert(b->row_arr[0].int_arr == NULL);
    assert(b->row_arr[1].r_size == 0);
    assert(b->row_arr[1].int_arr == NULL);
    assert(strcmp(
        str, //19
        "{}"
        "{}"
        "{[3]=6 [6]=9}"
        "{[12]=15}"
        "{[24]=27}"
        "{[48]=51}"
        "{[96]=99}"
        "{[192]=195}"
        "{[384]=387}"
        "{[768]=771}"
        "{[1536]=1539}"
        "{[3072]=3075}"
        "{[6144]=6147}"
        "{[12288]=12291}"
        "{[24576]=24579}"
        "{[49152]=49155}"
        "{[98304]=98307}"
        "{[196608]=196611}"
        "{[393216]=393219}")==0);
    assert(get_max_filled_row(b)==18);

    // delete all rows except last
    for(int i = 3; i < 393216; i*=2){
        if(bsa_get(b, i)){
            assert(bsa_delete(b, i));
        }
        assert(bsa_delete(b, i) == false);
        assert(bsa_get(b,i)==NULL);
    }
    assert(get_max_filled_row(b)==18);
    bsa_tostring(b, str);
    assert(strcmp(str,
                  "{}"
                  "{}"
                  "{}"
                  "{}"
                  "{}"
                  "{}"
                  "{}"
                  "{}"
                  "{}"
                  "{}"
                  "{}"
                  "{}"
                  "{}"
                  "{}"
                  "{}"
                  "{}"
                  "{}"
                  "{}"
                  "{[393216]=393219}") == 0);
    assert(get_max_filled_row(b)==18);
    assert(bsa_delete(b, 393216));
    // check that no rows exist
    for(int i = 0; i < BSA_ROWS; i++){
        assert(b->row_arr[i].int_arr == NULL);
    }
    bsa_free(b);
}

void* acalloc(int n, size_t size) 
{
    void* v = calloc(n, size);
    if(v == NULL){
        on_error("Cannot calloc() space");
    }
    return v;
}

void on_error(const char* s) 
{
    fprintf(stderr, "%s\n", s);
    exit(EXIT_FAILURE);
}

void add(int* p, int* n) 
{
    // Need to use n to switch off warnings :-(
    *n = 0;
    *p = *p + 1;
}