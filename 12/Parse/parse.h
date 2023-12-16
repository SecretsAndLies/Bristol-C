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
#define MAX_ARGS 2
#define MIN_FILENAME_LEN 4    // must be at least 4 characters (ie: a.ttl)

#define DEBUG //printf("%s %s %i\n",CURRENT_WORD, __func__, __LINE__);

typedef struct Program{
   char words[MAXNUMTOKENS][MAXTOKENSIZE];
   int curr_word;
} Program;

void* nfopen(char* fname, char* mode);
void on_error(const char* s);

Program * get_program(char * prog_name);

bool parse_program(Program *p);
void test_parse_program(void);

bool parse_inslst(Program *p);

bool parse_ins(Program *p);
void test_parse_ins(void);

bool parse_rgt(Program *p);
// tested in test_ins

bool parse_fwd(Program *p);
// tested in test_ins 

bool parse_num(Program *p);
void test_parse_num(void);

void test(void);

bool parse_col(Program *p);
void test_parse_col(void);

bool parse_loop(Program *p);
void test_parse_loop(void);

bool parse_set(Program *p);

bool check_ltr(char * s);
void test_check_ltr(void);

bool parse_var(Program *p);
void test_parse_var(void);

bool parse_word(Program *p);
void test_parse_word(void);

bool parse_lst(Program *p);

bool parse_items(Program *p);

bool parse_item(Program *p);

bool parse_varnum(Program * p);

bool is_operator(char c);
void test_is_operator(void);

bool parse_op(Program *p);
void test_parse_op(void);

bool is_number(char * str);
void test_is_number(void);

void check_args_valid(int argc, char *argv[]);


