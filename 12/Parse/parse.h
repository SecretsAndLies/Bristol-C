#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define strsame(A,B) (strcmp(A, B)==0)
#define ERROR(PHRASE) { fprintf(stderr, \
          "Fatal Error %s occurred in file: %s, function: %s, line %d\n", PHRASE, \
          __FILE__, __FUNC__, __LINE__); \
          exit(EXIT_FAILURE); }


void* nfopen(char* fname, char* mode);