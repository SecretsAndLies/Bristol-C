#pragma once
#include "../bsa.h"

#define LISTSTRLEN 1000

typedef struct int_s {
    int num; // the number.
    bool set; // if this element is set or not.
} int_s;

typedef struct row {
    int r_size; // how many elements are filled in this row
    int_s * int_arr; // list of int structures.
} row;

struct bsa {
    row row_arr[BSA_ROWS];
    int total_size;
};

void* ncalloc(int n, size_t size);
void on_error(const char* s);
int get_row_from_index(int indx);
void test_get_row_from_index(void);
int get_length_of_row(int row);
void test_get_length_of_row(void);
int get_col_within_row(int row, int index);
void test_get_col_within_row(void);
int get_orig_index_from_arr_index(int row_index, int col_index);
void test_get_orig_index_from_arr_index(void);
int get_max_row_which_contains_a_value(bsa* b);
void test_get_max_row_which_contains_a_value(void);
void test_bsa_init(void);
void test_bsa_set(void);
void test_bsa_get(void);
void test_bsa_delete(void);
void test_bsa_maxindex(void);
void test_bsa_tostring(void);
void append_string(char *destination, const char *source, size_t destSize);