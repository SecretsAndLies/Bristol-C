#include "../bsa.h"

// decent sized prime
#define SIZE 15319
#define LISTSTRLEN 1000
#define MAX_IN_LL 1000

typedef struct node {
    int index;
    int num;
    struct node* next;
} node;

struct bsa {
    node* buckets[SIZE];
};

void* acalloc(int n, size_t size);
void on_error(const char* s);
int hash(int index);
void add(int* p, int* n);