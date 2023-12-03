#include "../bsa.h"

#define INITIAL_SIZE 8
#define RESIZE_FACTOR 2

typedef struct int_s {
    int num; 
    bool set;
} int_s;

struct bsa {
    int_s *array;
    size_t size;
    size_t capacity;
};

void* acalloc(int n, size_t size);
void on_error(const char* s);