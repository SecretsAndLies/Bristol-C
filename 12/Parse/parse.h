#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define strsame(A,B) (strcmp(A, B)==0)
#define ERROR(PHRASE) { fprintf(stderr, \
          "Fatal Error %s occurred in file: %s, function: %s, line: %d\n", PHRASE, \
          __FILE__, __func__, __LINE__); \
          exit(EXIT_FAILURE); }

#define MAXNUMTOKENS 100
#define MAXTOKENSIZE 20

struct prog{
   char words[MAXNUMTOKENS][MAXTOKENSIZE];
   int curr_word;
};

typedef struct prog Program;

void* nfopen(char* fname, char* mode);
Program * get_program(char * prog_name);

bool parse_program(Program *p);
void test_parse_program(void);

bool parse_inslst(Program *p);

bool parse_ins(Program *p);
void test_parse_ins(void);

bool parse_rgt(Program *p);
// tested in test_parse_ins

bool parse_fwd(Program *p);
// tested in test_parse_ins 

bool parse_num(Program *p);
void test_parse_num(void);

void test(void);
