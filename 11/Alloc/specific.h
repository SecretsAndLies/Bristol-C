#pragma once
#include "../bsa.h"

struct bsa {
    row* ptr_arr[BSA_ROWS];
    int t_size;
};

struct row{
    int r_size;
    int * list;
}

void* ncalloc(int n, size_t size);
void on_error(const char* s);