#include "../bsa.h"

#define LISTSTRLEN 1000
#define MAX_INDEX 1073741822

typedef struct int_s {
    int num;
    bool set; 
} int_s;

typedef struct row {
    int r_size;
    int r_len; 
    int_s * int_arr;
} row;

struct bsa {
    row row_arr[BSA_ROWS];
    int total_size;
};

void* acalloc(int n, size_t size);
void on_error(const char* s);
int get_row_from_index(int indx);
void test_get_row_from_index(void);
int get_length_of_row(int row);
void test_get_length_of_row(void);
int get_col_within_row(int row, int index);
void test_get_col_within_row(void);
int get_orig_index_from_arr_index(int row_index,
                                  int col_index);
void test_get_orig_index_from_arr_index(void);
int get_max_filled_row(bsa* b);
void test_get_max_filled_row(void);
void test_bsa_init(void);
void test_bsa_set(void);
void test_bsa_get(void);
void test_bsa_delete(void);
void test_bsa_maxindex(void);
void test_bsa_tostring(void);
void append_string(char* dest, const char* source,
                   size_t destSize);
void test_add(void);
void add(int* p, int* n);
bool is_invalid_index(int indx);
void build_row_string(char* str, int r, bsa* b);
void add(int* p, int* n);