#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#define CURRENT_WORD p->words[p->curr_word]
#define FIRST_LETTER CURRENT_WORD[0]
#define strsame(A,B) (strcmp(A, B)==0)

#define MAXNUMTOKENS 1000
#define MAXTOKENSIZE 100

#define DEBUG //printf("%s %s %i\n",CURRENT_WORD, __func__, __LINE__);

typedef struct Program{
   char words[MAXNUMTOKENS][MAXTOKENSIZE];
   int curr_word;
} Program;

void* nfopen(char* fname, char* mode);
void on_error(const char* s);

Program * get_program(char * prog_name);

bool program(Program *p);
void test_program(void);

bool inslst(Program *p);

bool ins(Program *p);
void test_ins(void);

bool rgt(Program *p);
// tested in test_ins

bool fwd(Program *p);
// tested in test_ins 

bool num(Program *p);
void test_num(void);

void test(void);

bool col(Program *p);
void test_col(void);

bool loop(Program *p);
void test_loop(void);

bool set(Program *p);

bool check_ltr(char * s);
void test_check_ltr(void);

bool var(Program *p);
void test_var(void);

bool word(Program *p);
void test_word(void);

bool lst(Program *p);

bool items(Program *p);

bool item(Program *p);

bool varnum(Program * p);

bool is_operator(char c);
void test_is_operator(void);

bool op(Program *p);
void test_op(void);

bool is_number(char * str);
void test_is_number(void);