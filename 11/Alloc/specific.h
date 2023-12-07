#include "../bsa.h"

#define LISTSTRLEN 1000
#define OFFSET 2

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
int get_row(int indx);
void test_get_row(void);
int get_row_len(int row);
void test_get_row_len(void);
int get_col(int row, int index);
void test_get_col(void);
int get_index(int row, int col);
void test_get_index(void);
int get_max_filled_row(bsa* b);
void test_get_max_filled_row(void);
void test_bsa_init(void);
void test_bsa_set(void);
void test_bsa_get(void);
void test_bsa_delete(void);
void test_bsa_maxindex(void);
void test_bsa_tostring(void);
bool is_invalid_index(int indx);
void test_is_invalid_index(void);
void build_row_string(char* str, int r, bsa* b);
void test_bsa_foreach(void);
void test_is_invalid_input(void);
void add(int* p, int* n);
int get_max_index(int row_count);
void test_get_max_index(void);
