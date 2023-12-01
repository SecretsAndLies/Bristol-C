#pragma once
#include "../bsa.h"

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
